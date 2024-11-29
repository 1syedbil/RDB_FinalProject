#ifndef TRACKREVENUE
#define TRACKREVENUE

#include <stdio.h>
#include <mysql.h>
#include "input.h"


void trackRevenue(MYSQL* conn) {
	/*
	* Track Revenue from Purchases – Hyungseop
		Enter Item id
		Display total sales
	*/
	int item_id = 0;

	printf("Track Revnue from Purchase\n");
	getInteger("Enter Item ID: ", &item_id);

	//item id, item name, item quantity, total price
	//after the purchase happened, sum of specific item and display sum of total_price
	const char* query = "asasdsd";

	char queryStr[1024];
	snprintf(queryStr, sizeof(queryStr), query, item_id);

}


#endif