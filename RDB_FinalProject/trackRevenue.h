#ifndef TRACKREVENUE
#define TRACKREVENUE

#include <mysql.h>
#include "input.h"

#define QUERYLENGTH 1024

//item id, item name, item quantity, total revenue(sum of total price)
//after the purchase happened, sum of specific item and display sum of total_price

//prototypes
bool searchItemId(MYSQL* conn, int item_id);

void trackRevenue(MYSQL* conn) {
    MYSQL_RES* res;
    MYSQL_ROW row;

    while (true) {

        int choice = 0;
        int item_id = 0;

        printf("Welcom to Track Revnue from Purchase Screen\n");
        printf("Enter '1' to continue or '0' to go back: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Enter Itmed ID: ");
            scanf("%d", &item_id);
            
            if (searchItemId(conn, item_id)) {
                const char* query = "SELECT p.item_id,  i.`name`, sum(p.item_quantity) AS total_quantity, sum(p.total_price) AS total_revenue "
                    "FROM purchase_item p "
                    "JOIN item i WHERE p.item_id = i.id AND p.item_id = %d;";

                char queryStr[QUERYLENGTH];
                snprintf(queryStr, sizeof(queryStr), query, item_id);

                if (mysql_query(conn, queryStr)) {
                    fprintf(stderr, "QUERY failed: %s\n", mysql_error(conn));
                    return;
                }

                res = mysql_store_result(conn);
                if (res == NULL) {
                    fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
                    mysql_close(conn);
                }

                if (mysql_num_rows(res) > 0) {
                    printf("\nTotal Revenue of Customer ID: %d \n\n", item_id);

                    while ((row = mysql_fetch_row(res)) != NULL) {
                        printf("Item ID: %s | Item Name: %s | Total Quantity: %s | Total Rvg. $%s \n\n", row[0], row[1], row[2], row[3]);
                    }
                }
                mysql_free_result(res);
            }
            else {
                printf("\nItem not found. Please enter another Item\n\n");
            }   
        }
        else {
            printf("Going Back To Main Menu\n");
            break;
        }
    }
}

bool searchItemId(MYSQL* conn, int item_id) {
    MYSQL_RES* res;

    const char* query =
        "SELECT p.item_id "
        "FROM purchase_item p "
        "WHERE p.item_id = %d;";

    char queryStr[QUERYLENGTH];
    snprintf(queryStr, sizeof(queryStr), query, item_id);

    //execute query
    if (mysql_query(conn, queryStr)) {
        fprintf(stderr, "QUERY failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }

    //store data
    res = mysql_store_result(conn);
    if (res == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        mysql_close(conn);
        return false;
    }

    if (mysql_num_rows(res) > 0) {
        mysql_free_result(res);
        return true;
    }
    else {
        mysql_free_result(res);
        return false;
    }
}

#endif