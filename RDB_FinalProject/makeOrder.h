/*
* Filename:			makeOrder.h
* Project:			Final Project
* Programmer:		Curtis Wentzlaff
* Date:				December 3, 2024
* Description:		
*/

#ifndef CREATE_ORDER
#define CREATE_ORDER

#include <stdbool.h>
#include <mysql.h>

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

void displayCurrentOrder(char* prompt, char* name, int quantity);
void displayOrders(char* prompt, orderItem* firstItem);

MYSQL_RES* readItem(MYSQL* conn, char* name);
MYSQL_RES* createOrder(MYSQL* conn);
MYSQL_RES* createOrderItem(MYSQL* conn, orderItem* item, int orderId);


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

	MYSQL_RES* results = NULL;


	// Get User Input
	while (continueOrder) {

		// Get item name and validate
		displayCurrentOrder(prompt, stringInput, numInput);
		getString(prompt, stringInput);

		// Get Quantity
		displayCurrentOrder(prompt, stringInput, numInput);
		getInteger(prompt, numInput);

		results = readItem(conn, stringInput);
		if (results->row_count == 1) {

		}
		mysql_free_result(results);

		displayOrders(prompt, start);

		// Prompt user to continue adding items to order
		strcat(prompt, "\nAdd another item? (yes/no)");
		getString(prompt, stringInput);
		if (stringInput[0] == 'n') { continueOrder = false; }
	}

	// Prompt user to complete order
	strcat(prompt, "\nAdd another item? (yes/no)");
	getString(prompt, stringInput);
	if (stringInput[0] == 'n') { 
		printf("Discarding order...\nReturning to main menu...\n");
		system("PAUSE");
		return;
	}


	// Create the Order and its OrderItems
	results = createOrder(conn);

	int orderId = 0;

	mysql_free_result(results);

	current = start;
	while (current != NULL) {
		results = createOrderItem(conn, current, orderId);
		current = current->next;
	}
}


void displayCurrentOrder(char* prompt, char* name, int quantity) {
	strcpy(prompt, "");
	strcat(prompt, "==Add an item to the order==\n\n");
	strcat(prompt, "Item: ");
	if (*name != "") { strcat(prompt, name); }
	strcat(prompt, "\n");
}

void displayOrders(char* prompt, orderItem* firstItem) {
	strcpy(prompt, "");
	orderItem* current = firstItem;
	while (current != NULL) {
		strcat(prompt, "Name: ");
		strcat(prompt, "");
		strcat(prompt, "\tQuantity: ");
		strcat(prompt, "");
		strcat(prompt, "\tTotal Order Cost: ");
		strcat(prompt, "");
		strcat(prompt, "\n");
	}
}


//-----------------------------------------QUERY METHODS-----------------------------------------//

MYSQL_RES* readItem(MYSQL* conn, char* name) {
	char query[QUERY_MAX];
	strcpy(query, "");
	strcat(query, "SELECT * FROM item WHERE name=");
	strcat(query, name);

	return mysql_query(conn, query);
}

MYSQL_RES* createOrder(MYSQL* conn) {
	char* timestamp = NULL;

	char query[QUERY_MAX];
	snprintf(query, QUERY_MAX, "INSERT INTO order(Order_date, Store_id) VALUES ( %s , %d )", timestamp, STORE_ID);

	return mysql_query(conn, query);
}

MYSQL_RES* createOrderItem(MYSQL* conn, orderItem* item, int orderId) {
	char query[QUERY_MAX];
	snprintf(query, QUERY_MAX, "INSERT INTO order_item(Order_id, Item_id, Item_quantity, Order_total) VALUES ( %d , %d, %d, %lf)",
		orderId, item->item_ID, item->quantity, item->item_order_cost);

	return mysql_query(conn, query);
}

#endif