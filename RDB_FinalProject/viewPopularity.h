#ifndef POPULARITY
#define POPULARITY

#include "input.h"

#define ALL 1
#define SPECIFIC 2

#endif

//prototypes
int popularitySubMenu(void);
void viewByDep(MYSQL* conn);
void viewByRev(MYSQL* conn);
void viewByUnits(MYSQL* conn); 
int selectNumOfItemsListed(void);

int popularitySubMenu(void)
{
	int choice = 0;

	printf("What would you like to view item popularity by?\n\n\t1. By Department\n\t2. By Sales Revnue\n\t3. By Number of Units Sold\n");

	getInteger("Input choice (1-3) from menu", &choice);

	return choice;
}

void viewByDep(MYSQL* conn)
{
	int choice = selectNumOfItemsListed();

	if (choice == 0)
	{
		return;
	}
	else if (choice != 0)
	{
		return;
	}
}

void viewByRev(MYSQL* conn)
{
	int choice = selectNumOfItemsListed();

	if (choice == 0)
	{
		return;
	}
	else if (choice != 0)
	{
		return;
	}
}

void viewByUnits(MYSQL* conn)
{
	int choice = selectNumOfItemsListed();

	if (choice == 0)
	{
		return;
	}
	else if (choice != 0)
	{
		return;
	}
}

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

				getInteger("Enter the number of items you want to list:", &choice); 

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