#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "functions.h"

void deposit()
{
	struct Account acc;
	char acNo[10];
	FILE *fp, *temp;
	int found = 0;
	float amount;

	printf("\nNOTE : To exit from a step type '0' \n");

	printf("\nEnter the Account Number: ");
	scanf("%s", acNo);
	if (compareString(acNo) == 1)
		return;

	printf("Enter the amount to deposit: ");
	scanf("%f", &amount);
	if (compareFloat(amount) == 1)
		return;

	fp = fopen("data/bank.txt", "r");
	temp = fopen("data/temp.txt", "w");

	if (fp == NULL || temp == NULL)
	{
		printf("Error opening file!\n");
		return;
	}

	while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
	{
		if (strcmp(acc.account_no, acNo) == 0)
		{
			acc.balance += amount;
			writeHistory(acc.account_no, "DEPOSIT", amount); // To store data in history.txt
			found = 1;
			printf("Deposit successful. New balance: %.2f\n", acc.balance);
		}
		fprintf(temp, "%s,%s,%s,%d,%.2f,%s\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance, acc.open_date);
	}

	fclose(fp);
	fclose(temp);

	remove("bank.txt");
	rename("temp.txt", "bank.txt");

	if (!found)
	{
		printf("Account not found.\n");
	}
}

void withdraw()
{
	struct Account acc;
	char acNo[10];
	FILE *fp, *temp;
	int found = 0;
	float amount;

	printf("NOTE : To exit from a step type '0' \n");
	printf("\nEnter the Account Number: ");
	scanf("%s", acNo);
	if (compareString(acNo) == 1)
		return;

	printf("Enter the amount to withdraw: ");
	scanf("%f", &amount);
	if (compareFloat(amount) == 1)
		return;

	fp = fopen("data/bank.txt", "r");
	temp = fopen("data/temp.txt", "w");

	if (fp == NULL || temp == NULL)
	{
		printf("Error opening file!\n");
		return;
	}

	while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
	{
		if (strcmp(acc.account_no, acNo) == 0)
		{
			if (acc.balance >= amount)
			{
				acc.balance -= amount;
				writeHistory(acc.account_no, "WITHDRAW", amount); // To store data in history.txt
				printf("Withdrawal successful. New balance: %.2f\n", acc.balance);
			}
			else
			{
				printf("Insufficient balance. Withdrawal failed.\n");
			}
		}
		fprintf(temp, "%s,%s,%s,%d,%.2f,%s\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance, acc.open_date);
	}

	fclose(fp);
	fclose(temp);

	remove("bank.txt");
	rename("temp.txt", "bank.txt");

	if (!found)
	{
		printf("Account not found.\n");
	}
}

void transfer()
{
	struct Account acc;
	char fromAc[10], toAc[10];
	float amount;
	int foundFrom = 0, foundTo = 0;
	float fromBalance = 0, toBalance = 0;
	FILE *fp, *temp;

	printf("\nNOTE : To exit from a step type '0' \n");
	printf("\nEnter Sender's Account Number: ");
	scanf("%s", fromAc);
	if (compareString(fromAc) == 1)
		return;

	printf("Enter Recipient's Account Number: ");
	scanf("%s", toAc);
	if (compareString(toAc) == 1)
		return;

	printf("Enter amount to transfer: ");
	scanf("%f", &amount);
	if (compareFloat(amount) == 1)
		;

	fp = fopen("data/bank.txt", "r");
	temp = fopen("data/temp.txt", "w");
	if (fp == NULL || temp == NULL)
	{
		printf("Error opening file!\n");
		return;
	}

	while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f,%10[^\n]\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance, acc.open_date) == 6)
	{
		if (strcmp(acc.account_no, fromAc) == 0)
		{
			foundFrom = 1;
			if (acc.balance >= amount)
			{
				acc.balance -= amount;
				fromBalance = acc.balance;
			}
			else
			{
				printf("Insufficient balance in sender's account.\n");
				fclose(fp);
				fclose(temp);
				remove("temp.txt");
				return;
			}
		}
		if (strcmp(acc.account_no, toAc) == 0)
		{
			acc.balance += amount;
			toBalance = acc.balance;
		}
		fprintf(temp, "%s,%s,%s,%d,%.2f,%s\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance, acc.open_date);
	}

	if (foundFrom && foundTo) // To store data in history.txt
	{
		writeHistory(fromAc, "TRANSFER_OUT", amount);
		writeHistory(toAc, "TRANSFER_IN", amount);
	}

	fclose(fp);
	fclose(temp);

	if (!foundFrom)
		printf("Sender account not found.\n");
	else if (!foundTo)
		printf("Recipient account not found.\n");
	else
	{
		remove("bank.txt");
		rename("temp.txt", "bank.txt");
		printf("Transfer successful!\n");
		printf("Sender's new balance: %.2f \nRecipient's new balance: %.2f \n", fromBalance, toBalance);
		return;
	}
	remove("temp.txt");
}

float calculateInterest(const char *accountType, float principal, int days) {
	float rate = 0.0f, time = 0.0f;
	if (strcmp(accountType, "Saving") == 0) {
		rate = 6.0f;
		// Calculate interest daily
		time = days / 365.0f;
	} else if (strcmp(accountType, "FD") == 0) {
		rate = 7.0f;
		time = 5.0f; // 5 year
	} else if (strcmp(accountType, "RD") == 0) {
		rate = 7.5f;
		time = 1.0f; // 1 years
	} else {
		printf("Invalid account type!\n");
		return 0.0f;
	}
	// Simple interest formula
	return (principal * rate * time) / 100.0f;
}

void accountOperation()
{
	int choice;
	while (1)
	{
		printf("\n1. Deposit Money \n2. Withdraw Money \n3. Transfer Money \n4. Exit Menu\n");
		printf("\nEnter your choice : ");
		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			deposit();
			break;
		case 2:
			withdraw();
			break;
		case 3:
			transfer();
			break;
		case 4:
			printf("Exited...\n");
			return;
		default:
			printf("Wrong Choice!\n");
		}
	}
}
