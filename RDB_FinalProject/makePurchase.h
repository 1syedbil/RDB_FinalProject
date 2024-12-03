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
    PurchaseItem* purchase_list = NULL;
    char item_name[MAX_INPUT];
    int quantity;
    char confirm;
    double item_price;

    while (true) {
        system("cls");
        displayPurchaseList(purchase_list);
        getString("Enter the item name (or type 'done' to finish or 'cancel' to go back)", item_name);

        if (strcmp(item_name, "done") == 0) {
            if (!purchase_list) {
                printf("Cannot fulfill an empty purchase.\n");
                system("pause");
                continue;
            }

            printf("Do you want to confirm this purchase? (y/n): ");
            confirm = getchar();
            clearInputBuffer();

            if (confirm == 'y' || confirm == 'Y') {
                updateDatabase(conn, purchase_list);
                printf("Purchase completed successfully.\n");
                system("pause");
                break;
            }
            else {
                continue;
            }
        }

        if (strcmp(item_name, "cancel") == 0) {
            freePurchaseList(purchase_list);
            return;
        }

        getInteger("Enter the quantity", &quantity);

        if (checkItemAvailability(conn, item_name, quantity, &item_price)) {
            addPurchaseItem(&purchase_list, item_name, quantity, item_price * quantity);
        }
    }

    freePurchaseList(purchase_list);
}


bool checkItemAvailability(MYSQL* conn, const char* item_name, int quantity, double* item_price) {

    return true;
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
