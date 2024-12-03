#ifndef USER_INPUT
#define USER_INPUT

#pragma warning (disable: 4996)
#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>

#define MAX_INPUT 50

#define DEP_ID 101
#define EMP_ID 102
#define ITE_ID 103
#define ORD_ID 104
#define ORD_ITE_ID 105
#define POS_ID 106
#define PUR_ID 107
#define PUR_ITE_ID 108
#define STO_ID 109
#define SUP_ID 110

typedef void (*voidFunc)(void); // creates a function pointer type called voidFunc -- it can point to functions that return nothing and take no paramters.


// Takes care of the if statement when checking if a query was successful. 
inline void checkSqlQuery(MYSQL* conn, char* query)
{
	if (mysql_query(conn, query))
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		//mysql_close(conn);
		//exit(EXIT_FAILURE);
	}
}

// Takes care of the if statement for checking results and prints an error - assign return value to the result variable you work with.
inline MYSQL_RES* checkSqlRes(MYSQL* conn)
{
	MYSQL_RES* res = mysql_store_result(conn);
	if (!res)
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		return NULL;
	}
	return res;
}

void clearInputBuffer() {
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF) {}
}

void getInteger(const char inputPrompt[], int* result) {
	char input[MAX_INPUT];
	bool loop = true;
	while (loop) {
		printf("\n%s >> ", inputPrompt);
		fgets(input, MAX_INPUT - 1, stdin);

		/* Check for a input buffer overflow */
		if (strchr(input, '\n') == NULL) {
			clearInputBuffer();
		}

		if (sscanf(input, "%d", result) == 1) {
			loop = false;
		}
		else {
			printf("\n--ERROR: Input was not a valid value--\n");
		}
	}
}

void getDouble(const char inputPrompt[], double* result) {
	char input[MAX_INPUT];
	bool loop = true;
	while (loop) {
		printf("\n%s >> ", inputPrompt);
		fgets(input, MAX_INPUT - 1, stdin);

		/* Check for a input buffer overflow */
		if (strchr(input, '\n') == NULL) {
			clearInputBuffer();
		}

		if ((sscanf(input, "%lf", result) == 1) && *result >= 0.00) {
			loop = false;
		}
		else {
			printf("\n--ERROR: Input was not a valid value--\n");
		}
	}
}

void getString(const char inputPrompt[], char* result) {
	char input[MAX_INPUT];
	bool loop = true;
	printf("\n%s >> ", inputPrompt);
	fgets(input, (MAX_INPUT - 1), stdin);

	/* Check for a input buffer overflow */
	if (strchr(input, '\n') == NULL) {
		clearInputBuffer();
	}

	/* Remove newline character */
	input[strlen(input) - 1] = '\0';
	strcpy(result, input); // Copy input to result
}

inline bool checkRange(int toCheck, int min, int max)
{
	return (toCheck <= max && toCheck >= min); // Returns true if it is within the range
}

// Takes a function pointer, which should point to the menu you want to display. Will loop until it gets value within min/max range
// Can also just put small menus inside of the input prompt and pass in null for the menuPtr.
void getMenuChoice(const char inputPrompt[], int* result, int minChoice, int maxChoice, voidFunc menu) {
	char input[MAX_INPUT];
	bool loop = true;

	while (loop)
	{
		if (menu != NULL)
		{
			menu();
		}
		printf("\n%s >> ", inputPrompt);
		fgets(input, MAX_INPUT - 1, stdin);

		/* Check for a input buffer overflow */
		if (strchr(input, '\n') == NULL) {
			clearInputBuffer();
		}

		if ((!sscanf(input, "%d", result) == 1) || !checkRange(*result, minChoice, maxChoice)) {
			printf("\n--ERROR: Input was not a valid menu item--\n");
			system("pause");
			system("cls");
		}
		else
		{
			return; 
		}
	}
}

// Is used to find a table based upon an integer value
char* findTable(int where)
{
	switch (where)
	{
	case DEP_ID:
		return "department";
		break;
	case EMP_ID:
		return "employee";
		break;
	case ITE_ID:
		return "item";
		break;
	case ORD_ID:
		return "`order`";
		break;
	case ORD_ITE_ID:
		return "order_item";
		break;
	case POS_ID:
		return "position";
		break;
	case PUR_ID:
		return "purchase";
		break;
	case PUR_ITE_ID:
		return "purchase_item";
		break;
	case STO_ID:
		return "store";
		break;
	case SUP_ID:
		return "supplier";
		break;
	default:
		printf("Not a valid id type for this database");
		system("pause");
		return false;
		break;
	}
}

// Check a table specified by 'int where' - which should constant when passed - to see if an id exists
bool checkIfPrimaryKeyExists(MYSQL* conn, int where, int id) // Can only find primary keys for now
{
	char* table = findTable(where);
	char query[512];
	
	snprintf(query, sizeof(query), "SELECT COUNT(*) FROM %s WHERE id = %d", table, id);
	checkSqlQuery(conn, query);
	MYSQL_RES* res = checkSqlRes(conn);
	MYSQL_ROW row = mysql_fetch_row(res);

	if (atoi(row[0]) < 1)
	{
		return false;
	}

	return true;
}



#endif