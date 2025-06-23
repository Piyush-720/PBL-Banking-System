#include <stdio.h>
#include <time.h>
#include <string.h>
#include "struct.h"
#include "functions.h"

// Function to update interest and balance in bank.txt if eligible
void updateInterestIfEligible(const char *accNO)
{
	struct Account acc;
	float balance = 0;
	int found = 0;
	FILE *bankFp = fopen("data/bank.txt", "r");
	if (bankFp != NULL)
	{
		while (fscanf(bankFp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
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
		return;

	float interest = 0.0f;
	int eligible = 0;
	time_t now = time(NULL);
	struct tm open_tm = {0};
	sscanf(acc.open_date, "%4d-%2d-%2d", &open_tm.tm_year, &open_tm.tm_mon, &open_tm.tm_mday);
	open_tm.tm_year -= 1900;
	open_tm.tm_mon -= 1;
	time_t open_time = mktime(&open_tm);
	double days_passed = difftime(now, open_time) / (60 * 60 * 24);
	if (strcmp(acc.type, "saving") == 0 && days_passed >= 90)
	{
		eligible = 1;
		interest = calculateInterest("saving", balance, 90);
		balance += interest;
	}
	else if (strcmp(acc.type, "fd") == 0 && days_passed >= 365)
	{
		eligible = 1;
		interest = calculateInterest("fd", balance, 0);
		balance += interest;
	}
	else if (strcmp(acc.type, "rd") == 0 && days_passed >= 2555)
	{
		eligible = 1;
		interest = calculateInterest("rd", balance, 0);
		balance += interest;
	}
	if (eligible && interest > 0.0f)
	{
		FILE *inFp = fopen("data/bank.txt", "r");
		FILE *outFp = fopen("data/bank_tmp.txt", "w");
		if (inFp && outFp)
		{
			struct Account tempAcc;
			while (fscanf(inFp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", tempAcc.name, tempAcc.account_no, tempAcc.type, &tempAcc.pin, &tempAcc.balance, tempAcc.open_date) == 6)
			{
				if (strcmp(tempAcc.account_no, acc.account_no) == 0)
				{
					tempAcc.balance = balance;
				}
				fprintf(outFp, "%s,%s,%s,%d,%.2f,%s\n", tempAcc.name, tempAcc.account_no, tempAcc.type, tempAcc.pin, tempAcc.balance, tempAcc.open_date);
			}
			fclose(inFp);
			fclose(outFp);
			remove("data/bank.txt");
			rename("data/bank_tmp.txt", "data/bank.txt");
		}
		// Log interest as a transaction
		writeHistory(acc.account_no, "INTEREST", interest);
	}
}

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
		while (fscanf(bankFp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
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
				// Treat CREATE, DEPOSIT, TRANSFER_IN, INTEREST as Credit
				int isCredit = (strcmp(operation, "DEPOSIT") == 0 || strcmp(operation, "TRANSFER_IN") == 0 || strcmp(operation, "CREATE") == 0 || strcmp(operation, "INTEREST") == 0);
				printf("%-15s %-10.2f %-10s %-20s\n", operation, amount, isCredit ? "Credit" : "Debit", timestamp);
				if (strcmp(operation, "DEPOSIT") == 0 || strcmp(operation, "TRANSFER_IN") == 0 || strcmp(operation, "CREATE") == 0)
					totalDeposit += amount;
				else if (strcmp(operation, "WITHDRAW") == 0 || strcmp(operation, "TRANSFER_OUT") == 0)
					totalWithdraw += amount;
			}
		}
	}
	fclose(fp);

	float totalInterest = 0.0f;
	fp = fopen("data/history.txt", "r");
	if (fp != NULL)
	{
		while (fgets(line, sizeof(line), fp))
		{
			char fileAccNo[10], operation[20], timestamp[30];
			float amount;
			if (sscanf(line, "%9[^,],%19[^,],%f,%29[^\n]", fileAccNo, operation, &amount, timestamp) == 4)
			{
				if (strcmp(fileAccNo, accNO) == 0 && strcmp(operation, "INTEREST") == 0)
					totalInterest += amount;
			}
		}
		fclose(fp);
	}
	printf("\nTotal Deposited : %.2f \nTotal Withdrawn : %.2f \nCurrent Balance : %.2f \nTotal Interest Earned : %.2f\n", totalDeposit, totalWithdraw, balance, totalInterest);
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
	while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
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
	while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
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

// Scheduled function to update interest for all accounts
void scheduledInterestUpdate()
{
	FILE *inFp = fopen("data/bank.txt", "r");
	FILE *outFp = fopen("data/bank_tmp.txt", "w");
	if (!inFp || !outFp)
	{
		if (inFp)
			fclose(inFp);
		if (outFp)
			fclose(outFp);
		return;
	}
	struct Account acc;
	time_t now = time(NULL);
	while (fscanf(inFp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
	{
		float interest = 0.0f;
		int eligible = 0;
		struct tm open_tm = {0};
		sscanf(acc.open_date, "%4d-%2d-%2d", &open_tm.tm_year, &open_tm.tm_mon, &open_tm.tm_mday);
		open_tm.tm_year -= 1900;
		open_tm.tm_mon -= 1;
		time_t open_time = mktime(&open_tm);
		double days_passed = difftime(now, open_time) / (60 * 60 * 24);
		if (strcmp(acc.type, "Saving") == 0 && days_passed >= 90)
		{
			eligible = 1;
			interest = calculateInterest("Saving", acc.balance, 90);
			acc.balance += interest;
			strftime(acc.open_date, sizeof(acc.open_date), "%Y-%m-%d", localtime(&now));
		}
		else if (strcmp(acc.type, "FD") == 0 && days_passed >= 1825)
		{
			eligible = 1;
			interest = calculateInterest("FD", acc.balance, 0);
			acc.balance += interest;
			strftime(acc.open_date, sizeof(acc.open_date), "%Y-%m-%d", localtime(&now));
		}
		else if (strcmp(acc.type, "RD") == 0 && days_passed >= 365)
		{
			eligible = 1;
			interest = calculateInterest("RD", acc.balance, 0);
			acc.balance += interest;
			strftime(acc.open_date, sizeof(acc.open_date), "%Y-%m-%d", localtime(&now));
		}
		if (eligible && interest > 0.0f)
		{
			writeHistory(acc.account_no, "INTEREST", interest);
		}
		fprintf(outFp, "%s,%s,%s,%d,%.2f,%s\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance, acc.open_date);
	}
	fclose(inFp);
	fclose(outFp);
	remove("data/bank.txt");
	rename("data/bank_tmp.txt", "data/bank.txt");
}
