#include <stdio.h>
#include "functions.h"

void main()
{
    int choice;
    while (1)
    {
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