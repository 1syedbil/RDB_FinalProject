#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <mysql.h>

#include "input.h"


int main(void)
{
    MYSQL* conn;
    MYSQL_RES* res;
    MYSQL_ROW row;
    char* server = "sql5.freesqldatabase.com";
    char* user = "sql5746768";
    char* password = "BtgqLmSpNk";
    char* database = "sql5746768";
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server,
        user, password, database, 0, NULL, 0)) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    if (mysql_query(conn, "show tables")) {
        fprintf(stderr, "%s\n", mysql_error(conn));
        exit(1);
    }
    res = mysql_use_result(conn);
    printf("MySQL Tables in mysql database:\n");
    while ((row = mysql_fetch_row(res)) != NULL)
        printf("%s \n", row[0]);
    mysql_free_result(res);
    mysql_close(conn);

	return 0;
}