/*
* Filename:			makeOrder.h
* Project :			Final Project
* Programmer :		Curtis Wentzlaff
* Date :			December 3, 2024
* Description :
*/

#ifndef CREATE_ORDER
#define CREATE_ORDER

#include <time.h>

#include "input.h"

#define PROMPT_MAX		500
#define QUERY_MAX		1000

#define STORE_ID		1

//-------------------------------------------STRUCTS---------------------------------------------//

typedef struct orderItem {
	int item_ID;
	int quantity;
	double item_order_cost;
	struct orderItem* next;
} orderItem;


//------------------------------------------PROTOTYPES-------------------------------------------//

void displayCurrentOrder(char* prompt, char* name);
void displayOrders(char* prompt, orderItem* firstItem);

void setOrderItem(MYSQL_RES* results, orderItem* item, int quantity);

int readItem(MYSQL* conn, char* name);
int createOrder(MYSQL* conn);
int createOrderItem(MYSQL* conn, orderItem* item, int orderId);


//-----------------------------------------MAIN METHODS------------------------------------------//

void makeOrder(MYSQL* conn) {

	// Input variables
	bool continueOrder = true;						// Loop variable
	char prompt[PROMPT_MAX];						// Holds the prompts given to the user
	int numInput = 0;								// Holds the valide number inputs
	char stringInput[MAX_INPUT];					// Holds the valid string inputs
	

	// Linked list variables
	orderItem* start = NULL;						// First link in the linked list
	orderItem* current = NULL;						// Last added link in the linked list

	// Get User Input
	while (continueOrder) {

		// Get item name and validate
		strcpy(stringInput, "");
		system("CLS");
		displayCurrentOrder(prompt, stringInput);
		getString(prompt, stringInput);

		// Get Quantity
		system("CLS");
		displayCurrentOrder(prompt, stringInput);
		getInteger(prompt, &numInput);

		int res = readItem(conn, stringInput);
		MYSQL_RES* results = mysql_store_result(conn);
		if (results->row_count == 1) {
			if (start == NULL) {
				start = (orderItem*)malloc(sizeof(orderItem));
				setOrderItem(results, start, numInput);
				current = start;
			}
			else {
				current = current->next;
				current = (orderItem*)malloc(sizeof(orderItem));
				setOrderItem(results, current, numInput);
			}
		}
		mysql_free_result(results);

		//displayOrders(prompt, start);

		// Prompt user to continue adding items to order
		strcat(prompt, "\nAdd another item? (yes/no)");
		getString(prompt, stringInput);
		if (stringInput[0] == 'n') { continueOrder = false; }
	}

	// Prompt user to complete order
	strcpy(prompt, "");
	strcat(prompt, "\nConfirm Order? (yes/no)");
	getString(prompt, stringInput);
	if (stringInput[0] == 'n') {
		printf("Discarding order...\nReturning to main menu...\n");
		system("PAUSE");
		return;
	}

	// Create the Order and its OrderItems
	int res = createOrder(conn);
	MYSQL_RES* results = mysql_store_result(conn);
	int orderId = mysql_insert_id(conn);

	mysql_free_result(results);

	current = start;
	while (current != NULL) {
		int res = createOrderItem(conn, current, orderId);
		current = current->next;
	}

	current = start;
	while (current != NULL) {
		start = current->next;
		free(current);
		current = start;
	}


	system("PAUSE");
}


void setOrderItem(MYSQL_RES* results, orderItem* item, int quantity) {
	MYSQL_ROW* row = mysql_fetch_row(results);
	item->item_ID = atoi(row[0]);
	item->quantity = quantity;
	item->item_order_cost = (quantity * atof(row[3]));
	item->next = NULL;
}


void displayCurrentOrder(char* prompt, char* name) {
	strcpy(prompt, "");
	strcat(prompt, "==Add an item to the order==\n\n");
	strcat(prompt, "Item: ");
	if (strlen(name) != 0) { 
		strcat(prompt, name);
		strcat(prompt, "\n\nEnter the quantity");
	}
	else {
		strcat(prompt, "\n\nEnter the item name");
	}
}

//void displayOrders(char* prompt, orderItem* firstItem) {
//	strcpy(prompt, "");
//	orderItem* current = firstItem;
//	while (current != NULL) {
//		strcat(prompt, "Name: ");
//		strcat(prompt, "");
//		strcat(prompt, "\tQuantity: ");
//		strcat(prompt, "");
//		strcat(prompt, "\tTotal Order Cost: ");
//		strcat(prompt, "");
//		strcat(prompt, "\n");
//	}
//}


//-----------------------------------------QUERY METHODS-----------------------------------------//

int readItem(MYSQL* conn, char* name) {
	char query[QUERY_MAX];
	strcpy(query, "");
	strcat(query, "SELECT * FROM item WHERE `name`='");
	strcat(query, name);
	strcat(query, "'");

	return mysql_query(conn, query);
}

int createOrder(MYSQL* conn) {

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char query[QUERY_MAX];
	snprintf(query, QUERY_MAX, "INSERT INTO `order` (Order_date, Store_id) VALUES ('%d-%02d-%02d', 1)", 
		tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);

	return mysql_query(conn, query);
}

int createOrderItem(MYSQL* conn, orderItem* item, int orderId) {
	char query[QUERY_MAX];
	snprintf(query, QUERY_MAX, "INSERT INTO order_item (Order_id, Item_id, Item_quantity, Order_total) VALUES ( %d , %d, %d, %lf)", 
		orderId, item->item_ID, item->quantity, item->item_order_cost);

	return mysql_query(conn, query);
}

#endif