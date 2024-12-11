#pragma once
#ifndef MAKE_PURCHASE
#define MAKE_PURCHASE

/*
 * FILE          : makePurchase.h
 * PROJECT       : PROG2111 - Final Project
 * PROGRAMMER    : Navtej Saini (8958194)
 * FIRST VERSION : 2024-12-03
 * DESCRIPTION   :
 *   This file implements the "Make a Purchase" use case for the grocery store
 *   system. The functionality includes:
 *     - Validating item existence in the database.
 *     - Checking if the requested quantity of an item is available.
 *     - Adding items to a dynamically maintained purchase list.
 *     - Displaying the purchase list with calculated totals.
 *     - Updating the database with new purchases and decrementing inventory
 *       after purchase confirmation.
 */

#include "input.h"
#include <mysql.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

// Structs
typedef struct PurchaseItem {
    char item_name[MAX_INPUT];
    int quantity;
    double item_total;
    struct PurchaseItem* next;
} PurchaseItem;

// Function Prototypes
void makePurchase(MYSQL* conn);
void addPurchaseItem(PurchaseItem** head, const char* item_name, int quantity, double item_total);
void displayPurchaseList(PurchaseItem* head);
void updateDatabase(MYSQL* conn, PurchaseItem* head);
bool checkItemAvailability(MYSQL* conn, const char* item_name, int quantity, double* item_price);
bool checkItemExists(MYSQL* conn, const char* item_name, double* item_price);


/*
 * FUNCTION      : addPurchaseItem
 * DESCRIPTION   :
 *   Adds an item to the linked list representing the current purchase list.
 *   Each node in the linked list contains the item name, quantity, total price,
 *   and a pointer to the next node.
 * PARAMETERS    :
 *   PurchaseItem** head : A pointer to the pointer of the head of the purchase
 *                         list (used to add new items at the front).
 *   const char* item_name : The name of the item to be added to the list.
 *   int quantity : The quantity of the item being purchased.
 *   double item_total : The total cost of the item (quantity * unit price).
 * RETURNS       :
 *   void : This function does not return a value.
 */
void addPurchaseItem(PurchaseItem** head, const char* item_name, int quantity, double item_total) {
    PurchaseItem* new_item = (PurchaseItem*)malloc(sizeof(PurchaseItem));
    strcpy(new_item->item_name, item_name);
    new_item->quantity = quantity;
    new_item->item_total = item_total;
    new_item->next = *head;
    *head = new_item;
}

/*
 * FUNCTION      : displayPurchaseList
 * DESCRIPTION   :
 *   Displays the current purchase list, including each item's name, quantity,
 *   and total price. The function also calculates and displays the total cost
 *   of all items in the list.
 * PARAMETERS    :
 *   PurchaseItem* head : A pointer to the head of the purchase list.
 * RETURNS       :
 *   void : This function does not return a value.
 */
void displayPurchaseList(PurchaseItem* head) {
    double total = 0;
    printf("Current Purchase List:\n");
    printf("| %-30s | %-10s | %-10s |\n", "Item Name", "Quantity", "Item Total");
    printf("--------------------------------------------------------------\n");
    while (head) {
        printf("| %-30s | %-10d | $%-9.2f |\n", head->item_name, head->quantity, head->item_total);
        total += head->item_total;
        head = head->next;
    }
    printf("--------------------------------------------------------------\n");
    printf("Total: $%.2f\n", total);
}

/*
 * FUNCTION      : freePurchaseList
 * DESCRIPTION   :
 *   Frees the memory allocated for the linked list representing the purchase
 *   list. This function is called to prevent memory leaks when the
 *   purchase list is no longer needed.
 * PARAMETERS    :
 *   PurchaseItem* head : A pointer to the head of the purchase list.
 * RETURNS       :
 *   void : This function does not return a value.
 */
void freePurchaseList(PurchaseItem* head) {
    while (head) {
        PurchaseItem* temp = head;
        head = head->next;
        free(temp);
    }
}

/*
 * FUNCTION      : makePurchase
 * DESCRIPTION   :
 *   Handles the "Make a Purchase" use case. The function allows users to:
 *     - Enter item names and validate their existence in the database.
 *     - Enter a valid quantity and check inventory availability.
 *     - Add items to a dynamically maintained purchase list.
 *     - Confirm the purchase, update the database with new purchases, and
 *       decrement the inventory of purchased items.
 * PARAMETERS    :
 *   MYSQL* conn : Pointer to the MySQL connection object used for database
 *                 queries.
 * RETURNS       :
 *   void : This function does not return a value.
 */
void makePurchase(MYSQL* conn) {
    PurchaseItem* purchase_list = NULL; // Initialize empty purchase list
    char item_name[MAX_INPUT];
    int quantity = 0;
    char confirm = '\0';
    double item_price = 0.0;
    bool valid_item = false;

    while (true) {
        system("cls");
        displayPurchaseList(purchase_list);

        // Prompt for item name
        while (true) {
            getString("Enter the item name (or type 'done' to finish or 'cancel' to go back)", item_name);

            if (strcmp(item_name, "done") == 0) {
                if (!purchase_list) {
                    printf("\nCannot fulfill an empty purchase.\n");
                    system("pause");
                    return;
                }

                // Confirm the purchase
                printf("\nDo you want to confirm this purchase? (y/n): ");
                confirm = getchar();
                clearInputBuffer();

                if (confirm == 'y' || confirm == 'Y') {
                    updateDatabase(conn, purchase_list);
                    printf("\nPurchase completed successfully.\n");
                    system("pause");
                    freePurchaseList(purchase_list);
                    return;
                }
                else if(confirm == 'n' || confirm == 'N') {
                    printf("\nPurchase Cancelled. Returning to Main Menu...\n");
                    system("pause");
                    return; // Go back to the item name prompt
                }
            }

            if (strcmp(item_name, "cancel") == 0) {
                freePurchaseList(purchase_list);
                printf("\nReturning to Main Menu...\n");
                system("pause");
                freePurchaseList(purchase_list);
                return; // Exit the makePurchase menu and return to the main menu
            }

            // Check if the item exists
            valid_item = checkItemExists(conn, item_name, &item_price);
            if (!valid_item) {
                printf("\nItem '%s' does not exist. Please try again.\n", item_name);
                continue; // Prompt for item name again
            }

            // Valid item found, break out of the validation loop
            break;
        }

        // Prompt for quantity
        while (true) {
            getInteger("\nEnter the quantity", &quantity);

            if (quantity <= 0) {
                printf("\nInvalid quantity. Please enter a positive number.\n");
                continue; // Prompt for quantity again
            }

            if (!checkItemAvailability(conn, item_name, quantity, &item_price)) {
                // Not enough inventory
                continue; // Re-prompt for quantity
            }

            // Valid quantity, break out of the loop
            break;
        }

        // Add to the purchase list
        addPurchaseItem(&purchase_list, item_name, quantity, item_price * quantity);
    }
}

/*
 * FUNCTION      : checkItemAvailability
 * DESCRIPTION   :
 *   Validates whether the requested quantity of an item is available in the
 *   database. If the requested quantity exceeds the available stock, an error
 *   message is displayed.
 * PARAMETERS    :
 *   MYSQL* conn : Pointer to the MySQL connection object used for database
 *                 queries.
 *   const char* item_name : The name of the item being checked.
 *   int quantity : The quantity of the item requested by the user.
 *   double* item_price : Pointer to store the item's unit price if available.
 * RETURNS       :
 *   bool : Returns true if the item is available in the requested quantity,
 *          false otherwise.
 */

bool checkItemAvailability(MYSQL* conn, const char* item_name, int quantity, double* item_price) {
    char query[256];
    sprintf(query, "SELECT retail_price, store_quantity FROM item WHERE name = '%s'", item_name);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Database Error: %s\n", mysql_error(conn));
        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        printf("Item '%s' does not exist.\n", item_name); // this case is redundant now with additon of checkItemExists()
        mysql_free_result(res);
        return false;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    *item_price = atof(row[0]); // Fetch the retail price as a float
    int stock = atoi(row[1]);  // Fetch the store quantity as integer
    mysql_free_result(res);

    // Check if requested quantity exceeds available stock
    if (stock < quantity) {
        printf("Not Enough Inventory for '%s'. Available: %d, Requested: %d\n", item_name, stock, quantity);
        return false; //Not enough stock
    }

    return true; // Enough stock
}

/*
 * FUNCTION      : checkItemExists
 * DESCRIPTION   :
 *   Checks if an item with the given name exists in the database. If the item
 *   exists, the item's price is retrieved and stored in the provided pointer.
 * PARAMETERS    :
 *   MYSQL* conn : Pointer to the MySQL connection object used for database
 *                 queries.
 *   const char* item_name : The name of the item to be checked.
 *   double* item_price : Pointer to store the item's unit price if it exists.
 * RETURNS       :
 *   bool : Returns true if the item exists, false otherwise.
 */

bool checkItemExists(MYSQL* conn, const char* item_name, double* item_price) {
    char query[256];
    sprintf(query, "SELECT retail_price FROM item WHERE name = '%s'", item_name);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Database Error: %s\n", mysql_error(conn));
        return false;
    }

    MYSQL_RES* res = mysql_store_result(conn);
    if (!res || mysql_num_rows(res) == 0) {
        mysql_free_result(res);
        return false; // Item does not exist
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    *item_price = atof(row[0]); // Fetch the retail price
    mysql_free_result(res);

    return true; // Item exists
}

/*
 * FUNCTION      : updateDatabase
 * DESCRIPTION   :
 *   Updates the database after the user confirms their purchase. This function:
 *     - Inserts a new row into the `purchase` table to create a new purchase ID.
 *     - Inserts rows into the `purchase_item` table for each item in the
 *       purchase list, recording the quantity and total price.
 *     - Decrements the `store_quantity` of each purchased item in the `item`
 *       table.
 * PARAMETERS    :
 *   MYSQL* conn : Pointer to the MySQL connection object used for database
 *                 queries.
 *   PurchaseItem* head : A pointer to the head of the purchase list.
 * RETURNS       :
 *   void : This function does not return a value.
 */

void updateDatabase(MYSQL* conn, PurchaseItem* head) {
    char query[256];
    int purchase_id;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Insert into the purchase table and get the purchase_id
    sprintf(query, "INSERT INTO purchase (purchase_date, store_id) VALUES ('%d-%02d-%02d', 1)",
        tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Error inserting into purchase table: %s\n", mysql_error(conn));
        return;
    }
    purchase_id = mysql_insert_id(conn);


    // Loop through the purchase list
    while (head) {
        // Insert into purchase_item
        sprintf(query,
            "INSERT INTO purchase_item (purchase_id, item_id, item_quantity, total_price) "
            "SELECT %d, id, %d, (%f * %d) FROM item WHERE name = '%s'",
            purchase_id, head->quantity, head->item_total / head->quantity, head->quantity, head->item_name);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error inserting into purchase_item table: %s\n", mysql_error(conn));
            return;
        }

        // Update item inventory
        sprintf(query,
            "UPDATE item SET store_quantity = store_quantity - %d WHERE name = '%s'",
            head->quantity, head->item_name);

        if (mysql_query(conn, query)) {
            fprintf(stderr, "Error updating item table: %s\n", mysql_error(conn));
            return;
        }

        // Move to the next item in the list
        head = head->next;

    }
}

#endif
