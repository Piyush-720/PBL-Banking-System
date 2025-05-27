#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "struct.h"
#include "functions.h"

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

void createAccount()
{
    struct Account acc;
    FILE *fp, *acNumFile;
    int lastAcNo = 18290;

    acNumFile = fopen("data/accountNumber.txt", "r");
    if (acNumFile != NULL)
    {
        fscanf(acNumFile, "%d", &lastAcNo);
        fclose(acNumFile);
    }

    sprintf(acc.account_no, "%d", lastAcNo); // To pass the value from lastAcNo to acc.account_no

    printf("\nNOTE : To exit from a step type '0' \n");

    // Account Holder's name
    printf("\nEnter Account holder's name : ");
    scanf(" %[^\n]", acc.name);
    if (compareString(acc.name) == 1)
        return;

    int isValid = 0, i = 0;
    while (!isValid)
    {
        while (i < strlen(acc.name))
        {
            if (!isalpha(acc.name[i]) && acc.name[i] != ' ')
            {
                printf("Invalid name! , Use letters only. \n");
                isValid = 0;
                scanf(" %[^\n]", acc.name);
            }
            i++;
        }
        isValid = 1;
    }

    // Account type
    int choose;
    printf("\n1. Saving Account \n2. Current Account \n3. Fixed Deposit \n4. Recurring Deposit \n");
    printf("\nEnter the Account type : ");
    scanf("%d", &choose);
    switch (choose)
    {
    case 0:
        printf("Exited... \n");
        return;
    case 1:
        strcpy(acc.type, "Saving");
        break;
    case 2:
        strcpy(acc.type, "Current");
        break;
    case 3:
        strcpy(acc.type, "FD");
        break;
    case 4:
        strcpy(acc.type, "RD");
        break;
    default:
        printf("\nInvalid account type selected! \n");
    }

    // Account's PIN
    isValid = 0;
    printf("\nCreate a 4 digit Personal Identification Number(PIN) : ");
    while (!isValid)
    {
        scanf("%d", &acc.pin);
        if (compareInt(acc.pin) == 1)
            return;

        if (acc.pin < 1000 || acc.pin > 9999)
        {
            isValid = 0;
            printf("Enter a valid PIN. (It should be ecaxtly 4 digits) : ");
        }
        else
            isValid = 1;
    }

    isValid = 0;

    // Amount to add in the account
    printf("Enter the amount to add : ");
    while (!isValid)
    {
        scanf("%f", &acc.balance);
        if (compareFloat(acc.balance) == 1)
            return;
        if (acc.balance < 0 || acc.balance > 100000000)
        {
            isValid = 0;
            printf("Enter a valid amount! \n");
        }
        else
            isValid = 1;
    }

    // Printing account details
    printf("\nYour Account is created Successfully! \n\nYour Account details are : \n");
    printf("Name : %s \nAccount Number : %s \nAccount Type : %s \nPIN : %d \nBalance : %.2f \n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);

    writeHistory(acc.account_no, "CREATE", acc.balance); // To store data in history.txt

    fp = fopen("data/bank.txt", "a");

    if (fp == NULL)
    {
        printf("Error opening bank.txt for writing!\n");
        return;
    }

    fprintf(fp, "%s,%s,%s,%d,%.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
    fclose(fp);

    lastAcNo++;
    acNumFile = fopen("data/accountNumber.txt", "w");
    fprintf(acNumFile, "%d", lastAcNo);
    fclose(acNumFile);
}

void closeAccount()
{
    char acNo[10];
    struct Account acc;
    int found = 0;
    FILE *fp, *temp;

    printf("\nNOTE : To exit from a step type '0'  \n");
    printf("Enter the Account Number to close: ");
    scanf("%s", acNo);
    if (compareString(acNo) == 1)
        return;

    fp = fopen("data/bank.txt", "r");
    temp = fopen("data/temp.txt", "w");
    if (fp == NULL || temp == NULL)
    {
        printf("Error opening file! \n");
        return;
    }

    while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
    {
        if (strcmp(acc.account_no, acNo) == 0)
        {
            found = 1;
            writeHistory(acc.account_no, "CLOSE", acc.balance);
            printf("Account %s closed successfully. \n", acNo);
            continue; // Skip writing this account
        }
        // Write all other accounts to temp.txt
        fprintf(temp, "%s,%s,%s,%d,%.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
    }

    fclose(fp);
    fclose(temp);

    if (found)
    {
        remove("bank.txt");
        rename("temp.txt", "bank.txt");
    }
    else
    {
        remove("temp.txt");
        printf("Account not found.\n");
    }
}