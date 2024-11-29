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

#define MAKE_PURCHASE 1
#define MAKE_ORDER 2
#define TRACK_REVENUE_ITEM 3
#define VIEW_ITEM_POPULARITY 4
#define CHECK_ORDER_STATUS 5
#define MANAGE_EMPLOYEE 6
#define QUIT_PROGRAM 7


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
    MYSQL_RES* res;
    MYSQL_ROW row;
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
        getInteger("Input choice (1-6) from menu", &input);

        switch (input)
        {
        case MAKE_PURCHASE:

            break;
        case MAKE_ORDER:

            break;
        case TRACK_REVENUE_ITEM:

            break;
        case VIEW_ITEM_POPULARITY:

            break;
        case CHECK_ORDER_STATUS:

            break;
        case MANAGE_EMPLOYEE:

            break;
        case QUIT_PROGRAM:
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Invalid input, please try again\n");
            system("pause");
            break;
        }
    }

   
    mysql_close(conn);

	return 0;
}
