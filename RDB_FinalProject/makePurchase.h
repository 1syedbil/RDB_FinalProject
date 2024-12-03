#pragma once
#ifndef MAKE_PURCHASE
#define MAKE_PURCHASE
#include "input.h"
#include <mysql.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

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


// Add item to purchase list
void addPurchaseItem(PurchaseItem** head, const char* item_name, int quantity, double item_total) {
    PurchaseItem* new_item = (PurchaseItem*)malloc(sizeof(PurchaseItem));
    strcpy(new_item->item_name, item_name);
    new_item->quantity = quantity;
    new_item->item_total = item_total;
    new_item->next = *head;
    *head = new_item;
}

// Display the current purchase list
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

// Free the linked list memory
void freePurchaseList(PurchaseItem* head) {
    while (head) {
        PurchaseItem* temp = head;
        head = head->next;
        free(temp);
    }
}

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

        // Step 1: Prompt for item name
        while (true) {
            getString("Enter the item name (or type 'done' to finish or 'cancel' to go back)", item_name);

            if (strcmp(item_name, "done") == 0) {
                if (!purchase_list) {
                    printf("\nCannot fulfill an empty purchase.\n");
                    system("pause");
                    break;
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
                else {
                    break; // Go back to the item name prompt
                }
            }

            if (strcmp(item_name, "cancel") == 0) {
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

        // Step 2: Prompt for quantity
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

        // Step 3: Add to the purchase list
        addPurchaseItem(&purchase_list, item_name, quantity, item_price * quantity);
    }
}


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


void updateDatabase(MYSQL* conn, PurchaseItem* head) {
    char query[256];
    int purchase_id;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    while (head) {
        // Insert into purchase_item

        // Update item inventory
    }
}

#endif
