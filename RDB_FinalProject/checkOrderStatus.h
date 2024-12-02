#ifndef CHECKORDERSTATUS
#define CHECKORDERSTATUS

#include "input.h"



bool checkIfPrimaryKeyExists(MYSQL* conn, int where, int id);



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
			   "Arival date  : %s\n", row[0], row[1] ? row[1] : "NA");
		if (!row[1])
		{
			return false;
		}
		return true;
	}

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


#endif



