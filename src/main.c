#include <stdio.h>
#include <string.h>
#include "functions.h"

void main()
{
	int choice;
	while (1)
	{
		scheduledInterestUpdate();
		printf("\n\n------- GRAPHIC ERA BANK -------\n");
		printf("\n|\t1. Manager Mode \n|\t2. Clerk Mode \n|\t3. User Mode \n|\t4. Exit Menu\n");
		printf("\nEnter your choice: ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			manager();
			break;
		case 2:
			clerk();
			break;
		case 3:
			user();
			break;
		case 4:
			printf("Exited...\n");
			return;
		default:
			printf("Wrong choice!\n");
		}
	}
}

int compareInt(int input) // Function to exit in between without giving integer input
{
	if (input == 0)
	{
		printf("Exited... \n");
		return 1;
	}
	return 0;
}

int compareFloat(float input) // Function to exit in between without giving float input
{
	if (input == 0.0f)
	{
		printf("Exited... \n");
		return 1;
	}
	return 0;
}

int compareString(char *input) // Function to exit in between without giving string input
{
	if (strcmp(input, "0") == 0)
	{
		printf("Exited... \n");
		return 1;
	}
	return 0;
}
