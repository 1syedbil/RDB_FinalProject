#ifndef CHECKORDERSTATUS
#define CHECKORDERSTATUS

#include "input.h"
#include <time.h>



bool checkIfPrimaryKeyExists(MYSQL* conn, int where, int id);

char* getCurrTimeStamp()
{
	static char currTime[20];
	time_t t = time(NULL);
	struct tm* tm_info = localtime(&t);
	strftime(currTime, sizeof(currTime), "%Y-%m-%d %H:%M:%S", tm_info);
	return currTime;
}



bool queryOrderStatus(MYSQL* conn, int orderId)
{
	char query[512];
	snprintf(query, sizeof(query),
		"SELECT Order_date, Arrival_date FROM `order` WHERE id = %d", orderId);

	checkSqlQuery(conn, query);
	MYSQL_RES* res = checkSqlRes(conn);
	MYSQL_ROW row = mysql_fetch_row(res);
	if (row)
	{
		printf("Order created: %s\n"
			   "Arival date  : %s\n", row[0], 
				row[1] ? row[1] : "NA"); // Argument for arrival date if row[1] == NULL - NA, else row[1] is displayed
		if (!row[1])
		{
			return false;
		}
		return true;
	}
}


// returns a pointer to an array of order data. 
void queryOrderItems(MYSQL* conn, int orderId)
{
	char query[512];	   // Main queries
	char* bigQuery = NULL;

	snprintf(query, sizeof(query), "SELECT id, Item_id, Item_quantity, Order_total FROM order_item WHERE Order_id = %d", orderId);
	checkSqlQuery(conn, query);
	MYSQL_RES* res = checkSqlRes(conn);
	MYSQL_ROW row = NULL;
		 // We now have an array of strings big enough to hold all of the subqueries for each item inside order_items related to the order_id requested. 
	  // Parse the results then execute subqueries on DB.
	int num = 0;
	int totalCost = 0;
	char miniQuery[100];
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		// [0] = id, [1] = Item_id, [2] = Item quantity, [3] = Order_total
		snprintf(miniQuery, sizeof(miniQuery), "UPDATE item SET store_quantity = store_quantity + %d WHERE id = %d;", atoi(row[2]), atoi(row[1]));
		checkSqlQuery(conn, miniQuery);
		num++;
		printf("Processed Order_Item #%d for Order with id %d\n", num, orderId);
		totalCost += atoi(row[3]);
	}
	if (totalCost != 0)
	{
		snprintf(miniQuery, sizeof(miniQuery), "UPDATE `order` SET Arrival_date = '%s' WHERE id = %d", getCurrTimeStamp(), orderId);
		checkSqlQuery(conn, miniQuery);
		system("pause");
	}
	printf("Order total cost is %d \n", totalCost);
	system("pause");
	mysql_free_result(res);

}

void checkOrderStatus(MYSQL* conn)
{
	bool loop = true;
	int orderId = 0;
	while (loop)
	{
		int choice = 0;
		system("cls");
		getMenuChoice("1 - View Order Status\n"
			"2 - Return to Main Menu\n"
			"Input choice ", &choice, 1, 2, NULL);
		switch (choice)
		{
		case 1: 
			getInteger("Input order id ", &orderId);
			if (checkIfPrimaryKeyExists(conn, ORD_ID, orderId))
			{
				queryOrderStatus(conn, orderId);
				system("pause");
			}
			else
			{
				printf("ERROR: order with id '%d' does not exist", orderId);
				system("pause");
			}
			break;
		case 2:
			return;
			break;
		default:
			break;
		}
	}
}

void recieveOrder(MYSQL* conn)
{
	bool loop = true;
	int orderId = 0;
	while (loop)
	{
		int choice = 0;
		system("cls");
		getMenuChoice("1 - Recieve Order\n"
			"2 - Return to Main Menu\n"
			"Input choice ", &choice, 1, 2, NULL);
		switch (choice)
		{
		case 1:
			getInteger("Input order id ", &orderId);
			if (checkIfPrimaryKeyExists(conn, ORD_ID, orderId))
			{
				if (queryOrderStatus(conn, orderId))
				{
					printf("ERROR: order with id '%d' has been recieved", orderId);
					system("pause");
					return;
				}
				queryOrderItems(conn, orderId);
				
			}
			else
			{
				printf("ERROR: order with id '%d' does not exist", orderId);
				system("pause");
			}
			break;
		case 2:
			return;
			break;
		default:
			break;
		}
	}
}


#endif



