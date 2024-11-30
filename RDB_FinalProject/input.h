#ifndef USER_INPUT
#define USER_INPUT

#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>

#define MAX_INPUT 50

typedef void (*voidFunc)(void); // creates a function pointer type called voidFunc -- it can point to functions that return nothing and take no paramters.

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

bool checkRange(int toCheck, int min, int max)
{
	return (toCheck <= max && toCheck >= min); // Returns true if it is within the range
}

// Takes a function pointer, which should point to the menu you want to display. Will loop until it gets value within min/max range
void getMenuChoice(const char inputPrompt[], int* result, int minChoice, int maxChoice, voidFunc menu) {
	char input[MAX_INPUT];
	bool loop = true;

	while (loop)
	{
		menu();
		printf("\n%s >> ", inputPrompt);
		fgets(input, MAX_INPUT - 1, stdin);

		/* Check for a input buffer overflow */
		if (strchr(input, '\n') == NULL) {
			clearInputBuffer();
		}

		if ((!sscanf(input, "%d", result) == 1) || !checkRange(*result, minChoice, maxChoice)) {
			printf("\n--ERROR: Input was not a valid value--\n");
			system("pause");
			system("cls");
		}
		else
		{
			return; 
		}
	}
}

#endif