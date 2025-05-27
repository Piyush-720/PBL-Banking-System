#include <stdio.h>
#include <string.h>
#include "functions.h"

void backupData()
{
    FILE *copy, *paste;
    char line[256];

    copy = fopen("data/bank.txt", "r");
    paste = fopen("data/bankBackup.txt", "w");
    if (copy == NULL)
    {
        printf("Error opening original file!\n");
        fclose(copy);
        return;
    }
    if (paste == NULL)
    {
        printf("Error opening backup file!\n");
        fclose(paste);
        return;
    }

    while (fgets(line, sizeof(line), copy))
    {
        fputs(line, paste);
    }

    fclose(copy);
    fclose(paste);
    printf("Backup Complete!\n");
}

void restoreData()
{
    FILE *copy = fopen("data/bankBackup.txt", "r");
    FILE *paste = fopen("data/bank.txt", "w");
    char line[256];

    if (copy == NULL)
    {
        printf("Error opening backup file!\n");
        fclose(copy);
        return;
    }
    if (paste == NULL)
    {
        printf("Error opening original file to restore!\n");
        fclose(paste);
        return;
    }

    while (fgets(line, sizeof(line), copy))
    {
        fputs(line, paste);
    }

    fclose(copy);
    fclose(paste);
    printf("Restore Complete!\n");
}

void manager()
{
    char defUsername[10], defPassword[10];
    char username[10], password[10];
    FILE *fp;

    fp = fopen("data/credentials.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening credentials file!\n");
        return;
    }

    printf("\nNOTE : To exit from a step type '0' \n");
    printf("\nEnter the username: ");
    scanf("%s", username);
    if (compareString(username) == 1)
        return;

    fscanf(fp, "%9[^,], %9s", defUsername, defPassword);
    fclose(fp);

    if (strcmp(defUsername, username) != 0)
    {
        printf("Wrong Username!\n");
        return;
    }

    printf("Enter the password: ");
    scanf("%s", password);
    if (compareString(password) == 1)
        return;

    if (strcmp(defPassword, password) != 0)
    {
        printf("Wrong Password!\n");
        return;
    }

    int choice;
    while (1)
    {
        printf("\n\n  ---------- Manager Menu ---------- \n");
        printf("\n|\t1. Generate Reports \n|\t2. Backup Data\n|\t3. Restore Data\n|\t4. Exit Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            reports();
            break;
        case 2:
            backupData();
            break;
        case 3:
            restoreData();
            break;
        case 4:
            printf("Exited...\n");
            return;
        default:
            printf("Wrong Choice!\n");
        }
    }
}

void clerk()
{
    char defUsername[10], defPassword[10];
    char username[10], password[10];
    FILE *fp;

    fp = fopen("data/credentials.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening credentials file!\n");
        return;
    }

    printf("\nNOTE : To exit from a step type '0' \n");
    printf("\nEnter the username: ");
    scanf("%s", username);
    if (compareString(username) == 1)
        return;

    fscanf(fp, "%*[^\n]\n"); // skip first line of credentials.txt file
    fscanf(fp, "%9[^,], %9s", defUsername, defPassword);
    fclose(fp);

    if (strcmp(defUsername, username) != 0)
    {
        printf("Wrong Username!\n");
        return;
    }

    printf("Enter the password: ");
    scanf("%s", password);
    if (compareString(password) == 1)
        return;
    if (strcmp(defPassword, password) != 0)
    {
        printf("Wrong Password!\n");
        return;
    }

    int choice;
    while (1)
    {
        printf("\n\n ------------ Clerk Menu ------------ \n");
        printf("\n|\t1. Create Account \n|\t2. Account Operations \n|\t3. Bank Statement \n|\t4. Close Account \n|\t5. Display All Accounts \n|\t6. Transaction History \n|\t7. Exit Menu");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            accountOperation();
            break;
        case 3:
            generateBankStatement();
            break;
        case 4:
            closeAccount();
            break;
        case 5:
            displayAllAccountReport();
            break;
        case 6:
            transactionHistoryReport();
            break;
        case 7:
            printf("Exited...\n");
            return;
        default:
            printf("Wrong Choice!\n");
        }
    }
}

void user()
{
    char defUsername[10], defPassword[10];
    char username[10], password[10];
    FILE *fp;

    fp = fopen("data/credentials.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening credentials file!\n");
        return;
    }

    printf("\nNOTE : To exit from a step type '0' \n");
    printf("\nEnter the username: ");
    scanf("%s", username);
    if (compareString(username) == 1)
        return;

    fscanf(fp, "%*[^\n]\n"); // skip first line of credentials.txt file
    fscanf(fp, "%*[^\n]\n"); // skip second line of credentials.txt file
    fscanf(fp, "%9[^,], %9s", defUsername, defPassword);
    fclose(fp);

    if (strcmp(defUsername, username) != 0)
    {
        printf("Wrong Username!\n");
        return;
    }

    printf("Enter the password: ");
    scanf("%s", password);
    if (compareString(password) == 1)
        return;
    if (strcmp(defPassword, password) != 0)
    {
        printf("Wrong Password!\n");
        return;
    }

    int choice;
    while (1)
    {
        printf("\n\n   ---------- User Menu ---------- \n");
        printf("\n|\t1. Create Account \n|\t2. Account Operations \n|\t3. Bank Statement \n|\t4. Close Account \n|\t5. Exit Menu\n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            createAccount();
            break;
        case 2:
            accountOperation();
            break;
        case 3:
            generateBankStatement();
            break;
        case 4:
            closeAccount();
            break;
        case 5:
            printf("Exited...\n");
            return;
        default:
            printf("Wrong Choice!\n");
        }
    }
}