#ifndef EMPLOYEE
#define EMPLOYEE

#include "input.h"

#define EMPLOYEESUB1 1
#define EMPLOYEESUB2 2
#define EMPLOYEESUB3 3

#endif

//prototypes
void manageEmployees(MYSQL* conn)
{
	bool loop = true;
	while (loop)
	{
		system("cls");
		int choice = 0;
		getMenuChoice("Manage Employees \n"
			" 1 - Add Employee\n" 
			" 2 - Remove Employee\n"
			" 3 - Update Employee Info\n"
			" 4 - Return to Main Menu\n"
			" Input choice", &choice, 1, 4, NULL);

		switch (choice)
		{
		case EMPLOYEESUB1:
			system("cls");

			system("pause");
			break;
		case EMPLOYEESUB2:
			system("cls");

			system("pause");
			break;
		case EMPLOYEESUB3:
			system("cls");

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