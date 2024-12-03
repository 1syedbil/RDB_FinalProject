#ifndef CREATE_ORDER
#define CREATE_ORDER

#include <stdbool.h>
#include <mysql.h>

#include "input.h"

#define PROMPT_MAX 500

//-------------------------------------------STRUCTS---------------------------------------------//

typedef struct orderItem {
	char item_name[MAX_INPUT];
	int quantity;
	double item_order_cost;
	struct orderItem* next;
} orderItem; 


//------------------------------------------PROTOTYPES-------------------------------------------//

void displayOrders(char* prompt, orderItem* firstItem);

MYSQL_RES* readItem(MYSQL* conn, char* name);
MYSQL_RES* createOrder(MYSQL* conn);
MYSQL_RES* createOrderItem(MYSQL* conn, orderItem* item);


//-----------------------------------------MAIN METHODS------------------------------------------//

void makeOrder(MYSQL* conn) {
	bool continueOrder = true;
	char prompt[PROMPT_MAX];

	orderItem* start = NULL;
	orderItem* current = NULL;

	while (continueOrder) {
		// prompt user for item name and order quantity

		// query for item

		// if valid add item to linked list
		// if not, display error to user

		displayOrders(prompt, start);

		// prompt user to continue adding items to order
	}
}

void displayOrders(char* prompt, orderItem* firstItem) {
	// Loop through each item, printing the information to the console
}


//-----------------------------------------QUERY METHODS-----------------------------------------//

MYSQL_RES* readItem(MYSQL* conn, char* name) {

}

MYSQL_RES* createOrder(MYSQL* conn) {

}

MYSQL_RES* createOrderItem(MYSQL* conn, orderItem* item) {

}

#endif
