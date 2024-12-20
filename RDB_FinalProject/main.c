/*
 * FILE          : main.c
 * PROJECT       : PROG2111 - Final Project
 * PROGRAMMER    : Dylan Shouldice-Jacobs (8915799)
 * FIRST VERSION : 2024-12-03
 * DESCRIPTION   : This is the testHarness for our project, it deals with collecting user input to determine what functionality to do.
 *				   Then it passes control to the functionality. As well as dealing with connecting to the mySQL server.
 */


#pragma warning (disable: 4996)


#include "input.h"
#include "makeOrder.h"
#include "checkOrderStatus.h"
#include "trackRevenue.h"
#include "viewPopularity.h"
#include "makePurchase.h"
#include "manageEmployee.h"

/* --- Menu Choices --- */

#define MAKE_PURCHASE 1
#define MAKE_ORDER 2
#define TRACK_REVENUE_ITEM 3
#define VIEW_ITEM_POPULARITY 4
#define CHECK_ORDER_STATUS 5
#define MANAGE_EMPLOYEE 6
#define REVIEVE_ORDER 7
#define QUIT_PROGRAM 8

#define POPULARITYSUB1 1
#define POPULARITYSUB2 2
#define POPULARITYSUB3 3
/* --- Main Menu --- */

#define MM_MAX_CHOICE 8
#define MM_MIN_CHOICE 1


/*
 * FUNCTION   : displayMainMenu
 * DESCRIPTION: Prints the main menu options.
 * PARAMETERS : VOID
 * RETURNS    : VOID
 */
void displayMainMenu()
{
	printf("1 - Make Purchase\n"
		"2 - Make Order\n"
		"3 - Track Revenue From Item\n"
		"4 - View Item Popularity\n"
		"5 - Check Order Status\n"
		"6 - Manage Employees\n"
		"7 - Recieve Order\n"
		"8 - Exit Program\n");
}


/*
 * FUNCTION   : connectToDataBase
 * DESCRIPTION: Connects the our mySQL server
 * PARAMETERS : MYSQL** conn - Memory address of the pointer that points to the MYSQL connection object
 * RETURNS    : VOID
 */
void connectToDatabase(MYSQL** conn)
{
	*conn = mysql_init(NULL);
	if (!mysql_real_connect(*conn, "sql5.freesqldatabase.com",
		"sql5746768", "BtgqLmSpNk", "sql5746768", 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(EXIT_FAILURE);
	}
}



int main(void)
{
    MYSQL* conn = NULL;
	connectToDatabase(&conn);	

	// Loop that keeps program alive and processes inputs, put your functions in here
	voidFunc menuPtr = displayMainMenu;
	bool loop = true;
	int input = 0;
	printf("--- Welcome to SQLMart, please enjoy your stay! ---");
	while (loop)
	{
		input = 0;
		system("cls");
		getMenuChoice("Input choice (1-8) from menu", &input, MM_MIN_CHOICE, MM_MAX_CHOICE, menuPtr);

		switch (input)
		{
		case MAKE_PURCHASE:
			system("cls");
			makePurchase(conn);

			break;
		case MAKE_ORDER:
			system("cls");
			makeOrder(conn);
			break;
		case TRACK_REVENUE_ITEM:
			system("cls");
			trackRevenue(conn);
			break;
		case VIEW_ITEM_POPULARITY:
			viewItemPopularity(conn);
			break;
		case CHECK_ORDER_STATUS:
			system("cls");
			checkOrderStatus(conn);
			break;
		case MANAGE_EMPLOYEE:
			manageEmployees(conn);
			break;
		case REVIEVE_ORDER:
			recieveOrder(conn);
			break;
		case QUIT_PROGRAM:
			loop = false;
			break;
		default:
			break;
		}
	}
    mysql_close(conn);

   return 0;
}
