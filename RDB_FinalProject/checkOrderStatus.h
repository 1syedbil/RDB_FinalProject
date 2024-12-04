/*
 * FILE          : checkOrderStatus.h
 * PROJECT       : PROG2111 - Final Project
 * PROGRAMMER    : Dylan Shouldice-Jacobs (8915799)
 * FIRST VERSION : 2024-12-03
 * DESCRIPTION   : This header file defines the neccesary functions to complete the tasks of checking an orders status, and recieving an order. 
 *				   It also does some verification to ensure these functionalities can be processed with the parameters passed. Such as checking
 *				   if the requested ID exists in the table, or seeing if the order has already been recieved. 
 */

#ifndef CHECKORDERSTATUS
#define CHECKORDERSTATUS

#include "input.h"
#include <time.h>

// These defines hold signify the indexes of an array, this is where these items will be stored after a query
#define ORDER_TOTAL  3
#define NUM_OF_ITEMS 2
#define ITEM_ID      1


/*
 * FUNCTION   : getCurrTimeStamp
 * DESCRIPTION: Returns the current date in the format "YYYY-MM-DD HH-MM-SS"
 * PARAMETERS : VOID
 * RETURNS    : Returns a string that holds the current time
 */
char* getCurrTimeStamp(void)
{
	static char currTime[20];
	time_t t = time(NULL);
	struct tm* tm_info = localtime(&t);
	strftime(currTime, sizeof(currTime), "%Y-%m-%d %H:%M:%S", tm_info);
	return currTime;
}


/*
 * FUNCTION   : queryOrderStatus
 * DESCRIPTION: Queries for the status of an order, as well as printing the Order_date and Arrival_date
 * PARAMETERS : MYSQL* conn - The pointer to the SQL connection object 
 *				int orderId - The orderId to check
 * RETURNS    : true if order is recieved, else false
 */
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
	mysql_free_result(res);
}


/*
 * FUNCTION   : queryOrderItems
 * DESCRIPTION: Is called when we want to recieve and order, it will only be called when we know the ID exists, so no need to check here.
 *				But it queries for all items in an order. Then it creates sub queries depending on what was found from the Order_item. 
 *				Then updates the item found quantity in the store.
 * PARAMETERS : MYSQL* conn - The pointer to the SQL connection object 
 *				int orderId - The orderId to check
 * RETURNS    : VOID
 */
void updateItemInventory(MYSQL* conn, int orderId)
{
	char query[512];	   // Main queries
	char* bigQuery = NULL;

	snprintf(query, sizeof(query), "SELECT id, Item_id, Item_quantity, Order_total FROM order_item WHERE Order_id = %d", orderId);
	checkSqlQuery(conn, query);
	MYSQL_RES* res = checkSqlRes(conn);
	MYSQL_ROW row = NULL;

	int num = 0;
	int totalCost = 0;
	char miniQuery[100];
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		snprintf(miniQuery, sizeof(miniQuery), "UPDATE item SET store_quantity = store_quantity + %d WHERE id = %d;", atoi(row[NUM_OF_ITEMS]), atoi(row[ITEM_ID]));
		checkSqlQuery(conn, miniQuery);
		num++;
		printf("Processed Order_Item #%d for Order with id %d\n", num, orderId);
		totalCost += atoi(row[ORDER_TOTAL]);
	}
	if (totalCost != 0)
	{
		snprintf(miniQuery, sizeof(miniQuery), "UPDATE `order` SET Arrival_date = '%s' WHERE id = %d", getCurrTimeStamp(), orderId);
		checkSqlQuery(conn, miniQuery);
	}
	printf("Order total cost is %d \n", totalCost);
	system("pause");
	mysql_free_result(res);
}


/*
 * FUNCTION   : checkOrderStatus
 * DESCRIPTION: Checks if an order has been, recieved by checking if the arrival date is NULL
 * PARAMETERS : MYSQL* conn - The pointer to the SQL connection object 
 * RETURNS    : VOID
 */
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


/*
 * FUNCTION   : recieveOrder
 * DESCRIPTION: Will recieve an order by calling updateItemInventory - which handles the actual updating. This function gets the input of the order to be recieved
 *				Then checks if it is a vlid ID inside of the table.
 * PARAMETERS : MYSQL* conn - The pointer to the SQL connection object 
 * RETURNS    : VOID
 */
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
				updateItemInventory(conn, orderId);
				
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



