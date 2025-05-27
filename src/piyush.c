#include <stdio.h>
#include <time.h>
#include <string.h>
#include "struct.h"
#include "functions.h"

void generateBankStatement()
{
    FILE *fp;
    char accNO[10], line[200];
    float totalDeposit = 0, totalWithdraw = 0, balance = 0;
    printf("Enter the Account Number : ");
    scanf("%s", accNO);

    // Find account holder's name and current balance
    struct Account acc;
    FILE *bankFp = fopen("data/bank.txt", "r");
    int found = 0;
    if (bankFp != NULL)
    {
        while (fscanf(bankFp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
        {
            if (strcmp(acc.account_no, accNO) == 0)
            {
                found = 1;
                balance = acc.balance;
                break;
            }
        }
        fclose(bankFp);
    }
    if (!found)
    {
        printf("Account not found!\n");
        return;
    }

    printf("\n------ Bank Statement ------\n");
    printf("Account Holder: %s \nAccount Number: %s \nAccount Type  : %s \n", acc.name, acc.account_no, acc.type);

    fp = fopen("data/history.txt", "r");
    if (fp == NULL)
    {
        printf("\nNo History Available!\n");
        return;
    }

    printf("\n%-15s %-10s %-10s %-20s\n", "Operation", "Amount", "Type", "Timestamp");
    while (fgets(line, sizeof(line), fp))
    {
        char fileAccNo[10], operation[20], timestamp[30];
        float amount;
        if (sscanf(line, "%9[^,],%19[^,],%f,%29[^\n]", fileAccNo, operation, &amount, timestamp) == 4)
        {
            if (strcmp(fileAccNo, accNO) == 0)
            {
                printf("%-15s %-10.2f %-10s %-20s\n", operation, amount, (strcmp(operation, "DEPOSIT") == 0 || strcmp(operation, "TRANSFER_IN") == 0) ? "Credit" : "Debit", timestamp);
                if (strcmp(operation, "DEPOSIT") == 0 || strcmp(operation, "TRANSFER_IN") == 0)
                    totalDeposit += amount;
                else if (strcmp(operation, "WITHDRAW") == 0 || strcmp(operation, "TRANSFER_OUT") == 0)
                    totalWithdraw += amount;
            }
        }
    }
    fclose(fp);

    printf("\nTotal Deposited : %.2f \nTotal Withdrawn : %.2f \nCurrent Balance : %.2f \n", totalDeposit, totalWithdraw, balance);
}


void writeHistory(const char *accNo, const char *operation, float amount)
{
    FILE *fp = fopen("data/history.txt", "a");
    if (fp == NULL)
        return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(fp, "%s,%s,%.2f,%04d-%02d-%02d %02d:%02d:%02d\n",
            accNo, operation, amount,
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
            t->tm_hour, t->tm_min, t->tm_sec);
    fclose(fp);
}

void transactionHistoryReport()
{
    FILE *fp;
    char accNO[10], line[200];

    printf("Enter the Account Number to show history: ");
    scanf("%s", accNO);

    fp = fopen("data/history.txt", "r");
    if (fp == NULL)
    {
        printf("\nNo History Available!\n");
        return;
    }

    printf("\nTransaction History for Account %s:\n", accNO);
    printf("%-10s %-15s %-10s %-20s\n", "AccNo", "Operation", "Amount", "Timestamp");
    int found = 0;
    while (fgets(line, sizeof(line), fp))
    {
        char fileAccNo[10], operation[20], timestamp[30];
        float amount;
        if (sscanf(line, "%9[^,],%19[^,],%f,%29[^\n]", fileAccNo, operation, &amount, timestamp) == 4)
        {
            if (strcmp(fileAccNo, accNO) == 0)
            {
                printf("%-10s %-15s %-10.2f %-20s\n", fileAccNo, operation, amount, timestamp);
                found = 1;
            }
        }
    }
    if (!found)
        printf("\nNo history found for this account.\n");
    fclose(fp);
}

void displayAllAccountReport()
{
    struct Account acc;
    FILE *fp;

    fp = fopen("data/bank.txt", "r");
    if (fp == NULL)
    {
        printf("No Data found!");
        return;
    }

    printf("\n\t\t---------- Accounts ----------\n\n");
    printf("%-20s %-12s %-12s %-11s %-15s\n\n", "Name", "Acc No", "Type", "PIN", "Balance");
    while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
    {
        printf("%-20s %-12s %-12s %-4d %15.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
    }
    fclose(fp);
}

void totalBankBalanceReport()
{
    FILE *fp = fopen("data/bank.txt", "r");
    if (fp == NULL)
    {
        printf("No Data found!\n");
        return;
    }
    struct Account acc;
    float total = 0;
    while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
    {
        total += acc.balance;
    }
    fclose(fp);
    printf("\nTotal Balance in the Bank: %.2f\n", total);
}

void dailyReport()
{
    FILE *fp = fopen("data/history.txt", "r");
    if (fp == NULL)
    {
        printf("No transaction history available!\n");
        return;
    }

    // Get today's date as string (YYYY-MM-DD)
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char today[11];
    snprintf(today, sizeof(today), "%04d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);

    char line[200], operation[20], timestamp[30];
    char accNo[10];
    float amount;
    int accountsCreated = 0, deposits = 0, withdrawals = 0;
    float totalDeposited = 0, totalWithdrawn = 0;

    while (fgets(line, sizeof(line), fp))
    {
        if (sscanf(line, "%9[^,],%19[^,],%f,%29[^\n]", accNo, operation, &amount, timestamp) == 4)
        {
            // Check if the transaction is from today
            if (strncmp(timestamp, today, 10) == 0)
            {
                if (strcmp(operation, "CREATE") == 0)
                {
                    accountsCreated++;
                }
                else if (strcmp(operation, "DEPOSIT") == 0)
                {
                    deposits++;
                    totalDeposited += amount;
                }
                else if (strcmp(operation, "WITHDRAW") == 0)
                {
                    withdrawals++;
                    totalWithdrawn += amount;
                }
            }
        }
    }
    fclose(fp);

    printf("\n------ Daily Report (%s) ------\n", today);
    printf("Accounts Created   : %d\n", accountsCreated);
    printf("Deposits           : %d\n", deposits);
    printf("Withdrawals        : %d\n", withdrawals);
    printf("Total Deposited    : %.2f\n", totalDeposited);
    printf("Total Withdrawn    : %.2f\n", totalWithdrawn);
}

void allTransactionHistoryReport()
{
    FILE *fp = fopen("data/history.txt", "r");
    char line[200];

    if (fp == NULL)
    {
        printf("No History Available!\n");
        return;
    }

    printf("\n\t---------- All Previous Transactions ----------\n");
    printf("%-10s %-16s %-18s %-20s\n", "AccNo", "Operation", "Amount", "Timestamp");

    int found = 0;
    while (fgets(line, sizeof(line), fp))
    {
        char accNo[10], operation[20], timestamp[30];
        float amount;
        if (sscanf(line, "%9[^,],%19[^,],%f,%29[^\n]", accNo, operation, &amount, timestamp) == 4)
        {
            printf("%-10s %-15s %-15.2f %-20s\n", accNo, operation, amount, timestamp);
            found = 1;
        }
    }
    if (!found)
        printf("No transactions found.\n");
    fclose(fp);
}

void reports()
{
    int choice;
    while (1)
    {
        printf("\n\n---------- Bank Reports ---------- \n");
        printf("\n1. Account Summary Report \n2. Transaction History Report \n3. Total Bank Balance Report \n4. Daily Summary Report \n5. Exit Menu \n");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayAllAccountReport();
            break;
        case 2:
            allTransactionHistoryReport();
            break;
        case 3:
            totalBankBalanceReport();
            break;
        case 4:
            dailyReport();
            break;
        case 5:
            printf("Exited...\n");
            return;
        default:
            printf("Wrong Choice!\n");
        }
    }
}
