#ifndef USER_INPUT_NEW
#define USER_INPUT_NEW

#pragma warning (disable: 4996)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <conio.h>

//#define MAX_INPUT 100

typedef bool (*validator)(char* input, void* output);

//void clearInputBuffer() {
//	int ch;
//	while ((ch = getchar()) != '\n' && ch != EOF) {}
//}

void getInput(const char prompt[], void* result, validator eval) {
	char input[MAX_INPUT];
	bool loop = true;

	while (loop) {
		system("CLS");
		printf("\n%s >> ", prompt);

		fgets(input, MAX_INPUT - 1, stdin);

		/* Check for a input buffer overflow */
		if (strchr(input, '\n') == NULL) {
			clearInputBuffer();
		}

		if (eval(&input, result)) {
			loop = false;
		}
		else {
			printf("\n--ERROR: Input was not a valid value--\n");
			system("PAUSE");
		}
	}
}

bool evalString(char* input, void* output) {
	return sscanf(input, "%s", output) == 1 ? true : false;
}

bool evalInt(char* input, void* output) {
	return sscanf(input, "%d", output) == 1 ? true : false;
}

bool evalMainMenu(char* input, void* output) {
	if (sscanf(input, "%d", output) == 1) {
		int value = *(int*)output;
		return value < 1 || value > 7 ? false : true;
	}
	return false;
}

#endif