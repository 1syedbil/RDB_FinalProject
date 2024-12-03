/*
 * FILE          : manageEmployee.h
 * PROJECT       : PROG2111 - Final Project
 * PROGRAMMER    : Bilal Syed (8927633)
 * FIRST VERSION : 2024-12-03
 * DESCRIPTION   : This header file defines the functions related to managing employee data in a grocery store database.
 *                 It includes functions for adding, removing, and updating employee information. It also provides functions 
                   to check the existence of employees, departments, and roles in the system.
 */

#ifndef EMPLOYEE
#define EMPLOYEE

#include "input.h"

#define EMPLOYEESUB1 1
#define EMPLOYEESUB2 2
#define EMPLOYEESUB3 3

#endif

//prototypes
void manageEmployees(MYSQL* conn); 
void addEmployee(MYSQL* conn); 
void removeEmployee(MYSQL* conn);
void updateEmployee(MYSQL* conn);
void displayEmployeeInfo(MYSQL* conn, char* id); 
bool checkDepExists(MYSQL* conn, int id); 
bool checkRoleExists(MYSQL* conn, char* role, char* department, char* employee);  
bool checkEmployeeExists(MYSQL* conn, int id); 
bool checkPositionExists(MYSQL* conn, int id);  
 
/*
 * FUNCTION      : manageEmployees()
 * DESCRIPTION   : This function handles the menu for employee management, offering options to add, remove, or update
 *                 employee information. It loops until the user chooses to return to the main menu.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
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
			addEmployee(conn); 

			system("pause");
			break;
		case EMPLOYEESUB2:
			system("cls");
			removeEmployee(conn);

			system("pause");
			break;
		case EMPLOYEESUB3:
			system("cls");
			updateEmployee(conn); 

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
 * FUNCTION      : addEmployee()
 * DESCRIPTION   : This function is used to add a new employee to the grocery store database. It prompts the user
 *                 for the employee's details, assigns the employee to a department and role, and stores the data 
                   in the appropriate tables. This function also allows the user to create multiple roles in different
				   departments for the user's newly created employee.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
void addEmployee(MYSQL* conn)
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL; 
	char name[MAXSTRING] = ""; 
	char roles[MAXSTRING][MAXSTRING] = { "" };
	char currentRole[MAXSTRING] = ""; 
	char departments[MAXSTRING][MAXSTRING] = { "" };
	char newEmployeeId[MAXSTRING] = ""; 
	char newPosId[MAXSTRING] = "";
	char query[MAXSTRING] = ""; 
	int id = 0;
	int depId = 0;
	int choice = 0;
	int counter = 0;

	getString("Enter a name for the new employee (or enter 'cancel' to go back)", &name); 

	if (strcmp(name, "cancel") != 0)
	{
		strcpy(query, "SELECT id FROM employee ORDER BY id DESC LIMIT 1"); 

		if (mysql_query(conn, query)) 
		{
			fprintf(stderr, "Error: %s\n", mysql_error(conn)); 
			return;
		}

		res = mysql_store_result(conn); 

		while (row = mysql_fetch_row(res)) 
		{
			strcpy(newEmployeeId, row[0]); 
		}

		id = atoi(newEmployeeId); 
		id++;
		sprintf(newEmployeeId, "%d", id);
		
		strcpy(query, "INSERT INTO employee(id, name, store_id) VALUES (");
		strcat(query, newEmployeeId); 
		strcat(query, ", '");
		strcat(query, name); 
		strcat(query, "', 1)");

		if (mysql_query(conn, query))
		{
			fprintf(stderr, "Error: %s\n", mysql_error(conn));
			return;
		}

		while (choice != 2)
		{
			strcpy(roles[counter], "'");
			system("cls");

			getInteger("Enter the ID of the department you are assigining the employee to", &depId); 

			if (!checkDepExists(conn, depId))
			{
				printf("A department corresponding with the ID you entered does not exist.\n\n");
				system("pause");
				continue; 
			}

			sprintf(departments[counter], "%d", depId);

			getString("\nEnter a role for your employee", &currentRole); 

			strcat(roles[counter], currentRole); 
			strcat(roles[counter], "'");

			if (!checkRoleExists(conn, roles[counter], departments[counter], newEmployeeId))  
			{
				strcpy(query, "SELECT id FROM position ORDER BY id DESC LIMIT 1");

				if (mysql_query(conn, query))
				{
					fprintf(stderr, "Error: %s\n", mysql_error(conn));
					return;
				}

				res = mysql_store_result(conn);

				while (row = mysql_fetch_row(res))
				{
					strcpy(newPosId, row[0]);
				}

				id = atoi(newPosId);
				id++;
				sprintf(newPosId, "%d", id);


				strcpy(query, "INSERT INTO position (id, employee_id, department_id, role_name) VALUES (");
				strcat(query, newPosId); 
				strcat(query, ",");
				strcat(query, newEmployeeId);
				strcat(query, ",");
				strcat(query, departments[counter]);
				strcat(query, ",");
				strcat(query, roles[counter]);
				strcat(query, ")");

				if (mysql_query(conn, query)) 
				{
					fprintf(stderr, "Error: %s\n", mysql_error(conn)); 
					return;
				}
			}
			else
			{
				printf("This employee is already assigned to the %s role in the department you specified.\n\n", roles[counter]);
				system("pause");
				continue;
			}

			getMenuChoice("Would you like to assign this employee to more roles/departments?\n1 - Yes\n2 - No\nInput choice", &choice, 1, 2, NULL); 

			counter++;
		}
	}
	else
	{
		return; 
	}


}

/*
 * FUNCTION      : removeEmployee()
 * DESCRIPTION   : This function removes an existing employee from the grocery store database. It prompts the user for the employee's
 *                 ID, verifies the employee's existence, and deletes their record from the database if confirmed.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
void removeEmployee(MYSQL* conn)
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char employeeId[MAXSTRING] = "";
	char query[MAXSTRING] = "DELETE FROM employee WHERE id="; 
	int id = 0;
	int choice = 0;

	getInteger("Enter the ID of the employee you would like to remove", &id); 
	sprintf(employeeId, "%d", id); 

	if (!checkEmployeeExists(conn, id)) 
	{
		printf("There is no employee corresponding with the ID you entered.\n\n");
		return;
	}
	else
	{
		strcat(query, employeeId); 
		getMenuChoice("Are you sure you want to remove this employee? If you proceed then all of the employee's information will be deleted.\n1 - Yes\n2 - No\nInput your choice", &choice, 1, 2, NULL);  

		if (choice == 1)
		{
			system("cls");

			if (mysql_query(conn, query))
			{
				fprintf(stderr, "Error: %s\n", mysql_error(conn));
				return;
			}

			printf("The employeed with the ID %d was deleted successfully.\n\n", id); 
			return; 
		}
		else if (choice == 2)
		{
			return;
		}
	}
}

/*
 * FUNCTION      : updateEmployee()
 * DESCRIPTION   : This function allows the user to update an existing employee's personal or position information. The user is prompted
 *                 to choose whether to update personal details or position details, and changes are saved to the database accordingly.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 * RETURNS       : None
 */
void updateEmployee(MYSQL* conn) 
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char employeeId[MAXSTRING] = "";
	char query[MAXSTRING] = "";
	char name[MAXSTRING] = "";
	char positionId[MAXSTRING] = "";
	char departmentId[MAXSTRING] = ""; 
	char role[MAXSTRING] = "";
	int id = 0;
	int choice = 0;
	bool loop = true; 

	getInteger("Enter the ID of the employee you would like to update", &id); 
	sprintf(employeeId, "%d", id); 

	if (!checkEmployeeExists(conn, id)) 
	{
		printf("There is no employee corresponding with the ID you entered.\n\n"); 
		return;
	}
	else
	{
		system("cls");

		displayEmployeeInfo(conn, employeeId); 

		getMenuChoice("What would you like to update?\n1 - Employee Personal Info\n2 - Employee Position Info\nInput your choice", &choice, 1, 2, NULL);

		if (choice == 1)
		{
			getString("Enter the name you would like to update the employee name to", &name); 

			strcpy(query, "UPDATE employee SET name = '");
			strcat(query, name); 
			strcat(query, "' WHERE id=");
			strcat(query, employeeId); 

			if (mysql_query(conn, query))
			{
				fprintf(stderr, "Error: %s\n", mysql_error(conn));
				return;
			}

			system("cls");
			printf("Employee info was successfully updated.\n\n");
			displayEmployeeInfo(conn, employeeId); 
			return; 
		}
		else if (choice == 2)
		{
			while (loop) 
			{
				getInteger("Enter the ID of the position you would like to update for the employee", &id); 

				if (!checkPositionExists(conn, id)) 
				{
					printf("There is no position corresponding with the ID you entered.\n\n"); 
					continue;
				}
				else
				{
					sprintf(positionId, "%d", id); 
					break; 
				}
			}

			while (loop)
			{
				getInteger("Enter the ID you would like to update the department ID to for this position", &id); 

				if (!checkDepExists(conn, id)) 
				{
					printf("There is no department corresponding with the ID you entered.\n\n");
					continue;
				}
				else
				{
					sprintf(departmentId, "%d", id);
					break;
				}
			}

			getString("Enter the role you would like to assign the employee to", &role); 

			strcpy(query, "UPDATE position SET department_id=");
			strcat(query, departmentId);
			strcat(query, ", role_name='");
			strcat(query, role); 
			strcat(query, "' WHERE id=");
			strcat(query, positionId); 

			if (mysql_query(conn, query))
			{
				fprintf(stderr, "Error: %s\n", mysql_error(conn));
				return;
			}

			system("cls");
			printf("Employee position info was successfully updated.\n\n");
			displayEmployeeInfo(conn, employeeId);
			return;
		}
	}
}

/*
 * FUNCTION      : displayEmployeeInfo()
 * DESCRIPTION   : This function displays all the information related to a specific employee, including their personal details and position
 *                 information (role, department). It queries the database and prints the employee's details in a readable format.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 *                 char* id - The ID of the employee whose information is to be displayed.
 * RETURNS       : None
 */
void displayEmployeeInfo(MYSQL* conn, char* id)
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char query[MAXSTRING] = "";

	printf("Here is all the information for the employee with ID %s:\n\n|     ID      |        Name        |    Store ID    |\n-----------------------------------------------------\n", id);

	strcpy(query, "SELECT * FROM employee WHERE id=");
	strcat(query, id);

	if (mysql_query(conn, query))
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		return;
	}

	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res)))
	{
		printf("| %-11.11s | %-18.18s | %-14.14s |\n\n", row[0], row[1], row[2]);
	}

	printf("Here are all of the employee's positions:\n\n|       ID       |     Employee ID    |  Department ID  |     Role     |\n------------------------------------------------------------------------\n");

	strcpy(query, "SELECT * FROM position WHERE employee_id =");
	strcat(query, id);

	if (mysql_query(conn, query))
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		return;
	}

	res = mysql_store_result(conn);

	while ((row = mysql_fetch_row(res)))
	{
		printf("| %-14.14s | %-18.18s | %-15.15s | %-12.12s |\n", row[0], row[1], row[2], row[3]);
	}
}

/*
 * FUNCTION      : checkPositionExists()
 * DESCRIPTION   : This function checks if a position record in the position table exists for a 
                   specific employee. The position table is used to keep track of what roles an 
				   employee has in different departments
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 *                 int id - The position ID to check.
 * RETURNS       : bool - Returns true if the position exists and false if it doesn't exist.
 */
bool checkPositionExists(MYSQL* conn, int id)
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char query[MAXSTRING] = "SELECT * FROM position WHERE id =";
	char posId[MAXSTRING] = "";

	sprintf(posId, "%d", id); 

	strcat(query, posId);  

	if (mysql_query(conn, query))
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		return false;
	}

	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		return false;
	}

	return true;
}

/*
 * FUNCTION      : checkRoleExists()
 * DESCRIPTION   : This function checks if a given role exists for a specific employee in a specific department.
 *				   This information will then be used to prevent an employee from having duplicate roles in a 
 *                 single department. This is because an employee can have the same role in two different departments
 *                 but can't have duplicate roles within a single department.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 *                 char* role - The role name to check.
 *                 char* department - The department ID.
 *                 char* employee - The employee ID.
 * RETURNS       : bool - Returns true if the role exists for the employee in the specified department and false if it
                          doesn't exist.
 */
bool checkRoleExists(MYSQL* conn, char* role, char* department, char* employee) 
{
	MYSQL_RES* res = NULL; 
	MYSQL_ROW row = NULL;
	char query[MAXSTRING] = "SELECT * FROM position WHERE employee_id ="; 

	strcat(query, employee); 
	strcat(query, " AND role_name =");
	strcat(query, role); 
	strcat(query, " AND department_id =");
	strcat(query, department); 

	if (mysql_query(conn, query))
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		return false;
	}

	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		return false;
	}

	return true;
}

/*
 * FUNCTION      : checkDepExists()
 * DESCRIPTION   : This function checks if a department exists in the database based on the given department ID.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 *                 int id - The ID of the department to check.
 * RETURNS       : bool - Returns true if the department exists and false if it doesn't exist.
 */
bool checkDepExists(MYSQL* conn, int id)  
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char dep[MAXSTRING] = "";
	char query[MAXSTRING] = "SELECT * FROM department WHERE id=";

	sprintf(dep, "%d", id); 
	strcat(query, dep); 

	if (mysql_query(conn, query))
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		return false; 
	}

	res = mysql_store_result(conn); 

	if (res->row_count == 0)
	{
		return false;  
	}

	return true;
}

/*
 * FUNCTION      : checkEmployeeExists()
 * DESCRIPTION   : This function checks if an employee exists in the database based on their employee ID.
 * PARAMETERS    : MYSQL* conn - MySQL connection object to interact with the database.
 *                 int id - The ID of the employee to check.
 * RETURNS       : bool - Returns true if the employee exists and flase if it doesn't exist.
 */
bool checkEmployeeExists(MYSQL* conn, int id)
{
	MYSQL_RES* res = NULL;
	MYSQL_ROW row = NULL;
	char dep[MAXSTRING] = "";
	char query[MAXSTRING] = "SELECT * FROM employee WHERE id="; 

	sprintf(dep, "%d", id);
	strcat(query, dep);

	if (mysql_query(conn, query))
	{
		fprintf(stderr, "Error: %s\n", mysql_error(conn));
		return false;
	}

	res = mysql_store_result(conn);

	if (res->row_count == 0)
	{
		return false;
	}

	return true;
}