#ifndef TRACKREVENUE
#define TRACKREVENUE

/*
* FILE : trackRevenue.h
* PROJECT : PROG2111 - FinalPorject 
* PROGRAMMER : Hyung Seop Lee
* FIRST VERSION : 2024-12-03
* DESCRIPTION :
* This program get user input of item ID and tracks
* the total revenue of that item and display
* itme ID, item Name, quantity and total revenue
*
*/

#include <mysql.h>
#include "input.h"

#define QUERYLENGTH 1024

//prototypes
bool searchItemId(MYSQL* conn, int item_id);

/*
 * FUNCTION: trackRevenue
 * DESCRIPTION:
 *   This function tracks the total revenue and quantity sold for a specific item ID
 *   by querying the database. The user can enter an item ID and view the details.
 * PARAMETERS:
 *   MYSQL* conn - Pointer to the MySQL connection object used to execute database queries.
 * RETURNS:
 *   None.
 */
void trackRevenue(MYSQL* conn) {
	MYSQL_RES* res;
	MYSQL_ROW row;

	while (true) {

		int choice = 0;
		int item_id = 0;

		getMenuChoice("Welcome to Track Revenue from Purchase Screen\n"
			"0 - Return to Main Menu\n"
			"1 - Contunue\n"
			"Input choice ", &choice, 0, 1, NULL);

		if (choice == 1) {
			getInteger("Enter Item ID", &item_id);

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
				printf("\nThere are no purchases made for the item corresponding with the ID you entered.\n\n");
			}
		}
		else {
			printf("Going Back To Main Menu\n");
			break;
		}
	}
}

/*
 * FUNCTION: searchItemId
 * DESCRIPTION:
 *   This function checks if a given item ID exists in the purchase_item table.
 * PARAMETERS:
 *   MYSQL* conn - Pointer to the MySQL connection object used to execute database queries.
 *   int item_id - The item ID to search for in the database.
 * RETURNS:
 *   bool - Returns true if the item ID exists, false otherwise.
 */

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