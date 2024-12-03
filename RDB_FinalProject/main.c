#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <mysql.h>

#include "input.h"
#include "curtis.h"
#include "checkOrderStatus.h"
#include "trackRevenue.h"
#include "viewPopularity.h"
#include "makePurchase.h"

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

#define MM_MAX_CHOICE 7
#define MM_MIN_CHOICE 1


void displayMainMenu()
{
	printf("1 - Make Purchase\n"
		"2 - Make Order\n"
		"3 - Track Revenue From Item\n"
		"4 - View Item Popularity\n"
		"5 - Check Order Status\n"
		"6 - Manage Employee's\n"
		"7 - Recieve Order\n"
		"8 - Exit Program\n");
}

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
	while (loop)
	{
		system("cls");
		getMenuChoice("Input choice (1-6) from menu", &input, MM_MIN_CHOICE, MM_MAX_CHOICE, menuPtr);

		switch (input)
		{
		case MAKE_PURCHASE:
			system("cls");
			makePurchase(conn);
			system("pause");
			break;
		case MAKE_ORDER:
			system("cls");

			break;
		case TRACK_REVENUE_ITEM:
			system("cls");
			trackRevenue(conn);
			break;
		case VIEW_ITEM_POPULARITY:
			
			while (input < 1 || input > 3)
			{
				system("cls");

				input = popularitySubMenu();

                switch (input)
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
                default:
                    printf("Invalid input, please try again\n");
                    system("pause");
                    break;
                }	
            }

			input = 0;
			
			break;
		case CHECK_ORDER_STATUS:
			system("cls");
			checkOrderStatus(conn);
			break;
		case MANAGE_EMPLOYEE:
			system("cls");

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
