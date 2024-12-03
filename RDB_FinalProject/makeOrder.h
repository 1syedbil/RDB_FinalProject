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

#define PROMPT_MAX 500

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
MYSQL_RES* createOrderItem(MYSQL* conn, orderItem* item);


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

	current = start;
	while (current != NULL) {
		results = createOrderItem(conn, current);
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
}


//-----------------------------------------QUERY METHODS-----------------------------------------//

MYSQL_RES* readItem(MYSQL* conn, char* name) {
	// query for item with specified name
}

MYSQL_RES* createOrder(MYSQL* conn) {
	// create new order using current date
}

MYSQL_RES* createOrderItem(MYSQL* conn, orderItem* item) {
	// create new order item using
}

#endif