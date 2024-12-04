/*
 * FILE          : viewPopularity.h
 * PROJECT       : PROG2111 - Final Project
 * PROGRAMMER    : Bilal Syed (8927633)
 * FIRST VERSION : 2024-12-03
 * DESCRIPTION   : This header file defines the functions related to viewing the popularity of items in a grocery store based on 
 *                 various metrics. It includes functions for viewing item popularity by department, by revenue, and by units sold.
 *                 It also provides functionality to select the number of items to be listed and the option to choose the popularity metric.
 */

#ifndef POPULARITY
#define POPULARITY

#include "input.h"

#define ALL 1
#define SPECIFIC 2
#define MAXSTRING 500

#define POPULARITYSUB1 1
#define POPULARITYSUB2 2
#define POPULARITYSUB3 3
#define DONE 4

#endif

//prototypes
void viewByDep(MYSQL* conn);
void viewByRev(MYSQL* conn);
void viewByUnits(MYSQL* conn);
int selectNumOfItemsListed(void);
void viewItemPopularity(MYSQL* conn); 
int chooseDepItemRevOrUnits(void);    

/*
 * FUNCTION      : viewItemPopularity()
 * DESCRIPTION   : This function displays the main menu for viewing item popularity. It offers options to view popularity by department, 
                   revenue, or units sold. It calls the appropriate function to display the results based on the user's input.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
void viewItemPopularity(MYSQL* conn)
{
	bool loop = true;
	while (loop)
	{
		system("cls");
		int choice = 0;
		getMenuChoice("View Item Popularity \n"
					  " 1 - By Department\n"
					  " 2 - By Sales Revenue\n"
					  " 3 - By Number of Units Sold\n"
					  " 4 - Return to Main Menu\n"
					  " Input choice", &choice, 1, 4, NULL);

		switch (choice)
		{
		case POPULARITYSUB1:
			system("cls");
			viewByDep(conn);

			system("pause");
			break;
		case POPULARITYSUB2:
			system("cls");
			viewByRev(conn);

			system("pause");
			break;
		case POPULARITYSUB3:
			system("cls");
			viewByUnits(conn);

			system("pause");
			break;
		case DONE:
			return;
			break;
		default:
			printf("Invalid input, please try again\n");
			system("pause");
			break;
		}
	}
}

/*
 * FUNCTION      : viewByDep()
 * DESCRIPTION   : This function allows the user to view item popularity by department. It prompts the user for a department ID and 
                   the metric (revenue or units) to determine popularity by.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
void viewByDep(MYSQL* conn) 
{ 
	MYSQL_RES* res = NULL; 
	MYSQL_ROW row = NULL; 
	int depID = 0; 
	char depName[MAXSTRING] = "";
	char id[MAXSTRING] = "";
	char query[MAXSTRING] = "SELECT name FROM department WHERE id=";
	int choice = selectNumOfItemsListed();
	char limit[MAXSTRING] = ""; 

	while (depID < 1)
	{
		getInteger("Enter the ID of the department you would like to view item popularity from", &depID);

		if (depID < 1)  
		{
			printf("Invalid input, please try again\n"); 
			system("pause"); 
			system("cls");
		}

		sprintf(id, "%d", depID);

		strcat(query, id);

		if (mysql_query(conn, query)) {
			fprintf(stderr, "%s\n", mysql_error(conn));  
			exit(1);  
		}

		res = mysql_store_result(conn);

		if (res->row_count == 0)
		{
			depID = 0;
			strcpy(query, "SELECT name FROM department WHERE id=");
			printf("A department corresponding with the ID you entered does not exist. Try again...\n\n");
			system("pause");
			system("cls");
		}
		else
		{
			break;
		}
	}

	row = mysql_fetch_row(res);

	strcpy(depName, row[0]);
	 
	mysql_free_result(res); 
	 
	if (choice == 0)
	{
		choice = chooseDepItemRevOrUnits();

		if (choice == 1)
		{
			printf("Here are the most popular items in the %s department by revenue:\n\n|     Item ID      |        Item Name         |    Total Rev    |\n-----------------------------------------------------------------\n", depName);

			strcpy(query, "SELECT purchase_item.item_id, item.name, SUM(purchase_item.total_price) AS Total_Sales_Rev FROM purchase_item JOIN item ON purchase_item.item_id = item.id JOIN department ON item.department_id = department.id WHERE department.id =");
			strcat(query, id); 
			strcat(query, " GROUP BY purchase_item.item_id ORDER BY SUM(purchase_item.total_price) DESC"); 

			if (mysql_query(conn, query)) { 
				fprintf(stderr, "%s\n", mysql_error(conn)); 
				exit(1); 
			}

			res = mysql_store_result(conn);

			while ((row = mysql_fetch_row(res)))
			{
				printf("| %-16.16s | %-24.24s | %-15.15s |\n", row[0], row[1], row[2]);
			}
		}
		else if (choice == 2)
		{
			printf("Here are the most popular items in the %s department by number of units sold:\n\n|     Item ID      |        Item Name         |    Total Units Sold    |\n------------------------------------------------------------------------\n", depName);

			strcpy(query, "SELECT purchase_item.item_id, item.name, SUM(purchase_item.item_quantity) AS Total_Units_Sold FROM purchase_item JOIN item ON purchase_item.item_id = item.id JOIN department ON item.department_id = department.id WHERE department.id =");
			strcat(query, id); 
			strcat(query, " GROUP BY purchase_item.item_id ORDER BY SUM(purchase_item.item_quantity) DESC");

			if (mysql_query(conn, query)) {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}

			res = mysql_store_result(conn);

			while ((row = mysql_fetch_row(res)))
			{
				printf("| %-16.16s | %-24.24s | %-22.22s |", row[0], row[1], row[2]);

				printf("\n");
			}
		}

		mysql_free_result(res);

		return;
	}
	else if (choice != 0)
	{
		sprintf(limit, "%d", choice); 

		choice = chooseDepItemRevOrUnits();

		if (choice == 1)
		{
			printf("Here are the most popular items in the %s department by revenue:\n\n|     Item ID      |        Item Name         |    Total Rev    |\n-----------------------------------------------------------------\n", depName);

			strcpy(query, "SELECT purchase_item.item_id, item.name, SUM(purchase_item.total_price) AS Total_Sales_Rev FROM purchase_item JOIN item ON purchase_item.item_id = item.id JOIN department ON item.department_id = department.id WHERE department.id =");
			strcat(query, id);
			strcat(query, " GROUP BY purchase_item.item_id ORDER BY SUM(purchase_item.total_price) DESC LIMIT ");
			strcat(query, limit); 

			if (mysql_query(conn, query)) {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}

			res = mysql_store_result(conn);

			while ((row = mysql_fetch_row(res)))
			{
				printf("| %-16.16s | %-24.24s | %-15.15s |", row[0], row[1], row[2]);

				printf("\n");
			}
		}
		else if (choice == 2)
		{
			printf("Here are the most popular items in the %s department by number of units sold:\n\n|     Item ID      |        Item Name         |    Total Units Sold    |\n------------------------------------------------------------------------\n", depName);

			strcpy(query, "SELECT purchase_item.item_id, item.name, SUM(purchase_item.item_quantity) AS Total_Units_Sold FROM purchase_item JOIN item ON purchase_item.item_id = item.id JOIN department ON item.department_id = department.id WHERE department.id =");
			strcat(query, id);
			strcat(query, " GROUP BY purchase_item.item_id ORDER BY SUM(purchase_item.item_quantity) DESC LIMIT ");
			strcat(query, limit); 

			if (mysql_query(conn, query)) {
				fprintf(stderr, "%s\n", mysql_error(conn));
				exit(1);
			}

			res = mysql_store_result(conn);

			while ((row = mysql_fetch_row(res)))
			{
				printf("| %-16.16s | %-24.24s | %-22.22s |", row[0], row[1], row[2]);

				printf("\n");
			}
		}

		mysql_free_result(res);

		return;
	}
}

/*
 * FUNCTION      : chooseDepItemRevOrUnits()
 * DESCRIPTION   : This function prompts the user to choose how they want to view item popularity in a department, either by revenue 
                   or by units sold.
 * PARAMETERS    : None
 * RETURNS       : int - The user's choice: 1 for revenue, 2 for units sold.
 */
int chooseDepItemRevOrUnits(void)
{
	int choice = 0;

	while (choice < 1 || choice > 2)
	{
		system("cls");

		printf("What would you like to view item popularity by for the department you chose?\n\t1. By Sales Revenue\n\t2. By Number of Units Sold\n");

		getInteger("Input choice (1-2) from menu", &choice);

		if (choice < 1 || choice > 2) 
		{
			printf("Invalid input, please try again\n");
			system("pause");
		}
	}

	system("cls");
	return choice; 
}

/*
 * FUNCTION      : viewByRev()
 * DESCRIPTION   : This function displays the most popular items in the store based on total revenue. The user can specify how many 
                   items to display.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
void viewByRev(MYSQL* conn)
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char limit[MAXSTRING] = "";  
	char query[MAXSTRING] = "SELECT purchase_item.item_id, item.name, SUM(purchase_item.total_price) AS Total_Sales_Rev FROM purchase_item JOIN item ON purchase_item.item_id = item.id GROUP BY purchase_item.item_id ORDER BY SUM(purchase_item.total_price) DESC"; 

	int choice = selectNumOfItemsListed();

	if (choice == 0)
	{
		printf("Here are the most popular items in the grocery store by revenue:\n\n|     Item ID      |        Item Name         |    Total Rev    |\n-----------------------------------------------------------------\n");

		if (mysql_query(conn, query)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)))
		{
			printf("| %-16.16s | %-24.24s | %-15.15s |", row[0], row[1], row[2]);

			printf("\n");
		}

		mysql_free_result(res); 

		return;
	}
	else if (choice != 0)
	{
		sprintf(limit, "%d", choice); 

		printf("Here are the most popular items in the grocery store by revenue:\n\n|     Item ID      |        Item Name         |    Total Rev    |\n-----------------------------------------------------------------\n");

		strcat(query, " LIMIT "); 
		strcat(query, limit); 

		if (mysql_query(conn, query)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)))
		{
			printf("| %-16.16s | %-24.24s | %-15.15s |", row[0], row[1], row[2]);

			printf("\n");
		}

		mysql_free_result(res); 

		return;
	}
}

/*
 * FUNCTION      : viewByUnits()
 * DESCRIPTION   : This function displays the most popular items in the store based on the number of units sold. The user can specify 
                   how many items to display.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
void viewByUnits(MYSQL* conn) 
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char limit[MAXSTRING] = "";
	char query[MAXSTRING] = "SELECT purchase_item.item_id, item.name, SUM(purchase_item.item_quantity) AS Total_Units_Sold FROM purchase_item JOIN item ON purchase_item.item_id = item.id GROUP BY purchase_item.item_id ORDER BY SUM(purchase_item.item_quantity) DESC";

	int choice = selectNumOfItemsListed();

	if (choice == 0)
	{
		printf("Here are the most popular items in the grocery store by number of units sold:\n\n|     Item ID      |        Item Name         |    Total Units Sold    |\n------------------------------------------------------------------------\n");

		if (mysql_query(conn, query)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)))
		{
			printf("| %-16.16s | %-24.24s | %-22.22s |", row[0], row[1], row[2]);

			printf("\n");
		}

		mysql_free_result(res);

		return;
	}
	else if (choice != 0)
	{
		sprintf(limit, "%d", choice); 

		printf("Here are the most popular items in the grocery store by number of units sold:\n\n|     Item ID      |        Item Name         |    Total Units Sold    |\n------------------------------------------------------------------------\n");

		strcat(query, " LIMIT ");
		strcat(query, limit); 
		
		if (mysql_query(conn, query)) {
			fprintf(stderr, "%s\n", mysql_error(conn));
			exit(1);
		}

		res = mysql_store_result(conn);

		while ((row = mysql_fetch_row(res)))
		{
			printf("| %-16.16s | %-24.24s | %-22.22s |", row[0], row[1], row[2]);

			printf("\n");
		}

		mysql_free_result(res); 

		return;
	}
}

/*
 * FUNCTION      : selectNumOfItemsListed()
 * DESCRIPTION   : This function asks the user how many items they want to display in the popularity list. It validates the input.
 * PARAMETERS    : None
 * RETURNS       : int - The number of items the user wants to list (0 for all items, or a positive number for a specific count).
 */
int selectNumOfItemsListed(void)
{
	int choice = 0; 

	while (choice < 1 || choice > 2)
	{
		printf("How many items would you like to list?\n\t1. All\n\t2. Specific Amount\n");

		getInteger("Input choice (1-2) from menu", &choice);

		switch (choice) 
		{
		case ALL:
			system("cls");
			return 0;
		case SPECIFIC:
			choice = 0;

			while (choice < 1)
			{
				system("cls");

				getInteger("Enter the number of items you want to list", &choice); 

				if (choice < 1)
				{
					printf("Invalid input, please try again\n");
					system("pause");
				}
			}

			system("cls");
			return choice;
		default:
			printf("Invalid input, please try again\n"); 
			system("pause"); 
			system("cls");
			break;
		}
	}
}