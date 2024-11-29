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

/* --- Menu Choices --- */

#define MAKE_PURCHASE 1
#define MAKE_ORDER 2
#define TRACK_REVENUE_ITEM 3
#define VIEW_ITEM_POPULARITY 4
#define CHECK_ORDER_STATUS 5
#define MANAGE_EMPLOYEE 6
#define QUIT_PROGRAM 7

#define POPULARITYSUB1 1
#define POPULARITYSUB2 2
#define POPULARITYSUB3 3
/* --- Main Menu --- */

#define MM_MAX_CHOICE 7
#define MM_MIN_CHOICE 1


void displayMainMenu()
{
    printf("1 - Make Purchase\n");
    printf("2 - Make Order\n");
    printf("3 - Track Revenue From Item\n");
    printf("4 - View Item Popularity\n");
    printf("5 - Check Order Status\n");
    printf("6 - Manage Employee's\n");
    printf("7 - Exit Program\n");
}

int main(void)
{
    // SQL Data
    MYSQL* conn;
    MYSQL_RES* res; //don't think this is neccessary in main - bilal
    MYSQL_ROW row;  //don't think this is neccessary in main - bilal
    char* server = "sql5.freesqldatabase.com";
    char* user = "sql5746768";
    char* password = "BtgqLmSpNk";
    char* database = "sql5746768";

    // Starting SQL Connection - Might put this all into a function so it looks nicer
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }


    // Loop that keeps program alive and processes inputs, put your functions in here
    int input = 0;
    while (input != 7)
    {
        system("cls");
        displayMainMenu();
        getMenuChoice("Input choice (1-6) from menu", &input, MM_MIN_CHOICE, MM_MAX_CHOICE);

        switch (input)
        {
        case MAKE_PURCHASE:
            system("cls");

            break;
        case MAKE_ORDER:
            system("cls");

            break;
        case TRACK_REVENUE_ITEM:
            system("cls"); 

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

                    break;
                case POPULARITYSUB2:
                    system("cls");  
                    viewByRev(conn); 

                    break;
                case POPULARITYSUB3: 
                    system("cls"); 
                    viewByUnits(conn); 

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

            break;
        case MANAGE_EMPLOYEE:
            system("cls"); 

            break;
        case QUIT_PROGRAM:
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
        }
    }

   
    mysql_close(conn);

	return 0;
}
