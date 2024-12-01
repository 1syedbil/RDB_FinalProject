#ifndef TRACKREVENUE
#define TRACKREVENUE

#include <mysql.h>
#include "input.h"

#define QUERYLENGTH 1024

//item id, item name, item quantity, total revenue(sum of total proce)
//after the purchase happened, sum of specific item and display sum of total_price

void trackRevenue(MYSQL* conn) {

	MYSQL_RES* res;
	MYSQL_ROW row;

	int item_id = 0;

	printf("Track Revnue from Purchase\n");
	getInteger("Enter Item ID: ", &item_id);

	const char* query = "asasdsd";

	char queryStr[QUERYLENGTH];
	snprintf(queryStr, sizeof(queryStr), query, item_id);

	if (mysql_query(conn, queryStr)) {
		fprintf(stderr, "QUERY failed: %s\n", mysql_error(conn));
		return;
	}

}


#endif