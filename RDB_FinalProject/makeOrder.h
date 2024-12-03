#ifndef CREATE_ORDER
#define CREATE_ORDER

#include <stdbool.h>

#include "input.h"

//-------------------------------------------STRUCTS---------------------------------------------//

typedef struct orderItem {
	char item_name[MAX_INPUT];
	int quantity;
	double item_order_cost;
	struct orderItem* next;
} orderItem;


//------------------------------------------PROTOTYPES-------------------------------------------//

void displayOrders() {

}


//------------------------------------------MAIN METHOD------------------------------------------//

void createOrder(MYSQL* conn) {
	bool continueOrder = true;
	struct orderItem* start;
	struct orderItem* current;

	while (continueOrder) {
		// prompt user for item name and order quantity

		// query for item

		// create new orderedItem struct link

		// display items currently in the order

		// prompt user to add more items or end
	}

	// create new order and save order id

	while (current != NULL) {
		// create an orderItem record
	}
}


//-----------------------------------------QUERY METHODS-----------------------------------------//

MYSQL_RES* readItem(char* name) {

}

MYSQL_RES* createOrder() {

}

MYSQL_RES* createOrderItem() {

}

#endif
