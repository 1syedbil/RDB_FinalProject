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