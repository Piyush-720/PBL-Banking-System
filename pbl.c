#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void createAccount();
void deposit();
void withdraw();
void transfer();
void accountOperation();
void generateBankStatement();
void writeHistory(const char *accNo, const char *operation, float amount);
void transactionHistoryReport();
void displayAllAccountReport();
void totalBankBalanceReport();
void dailyReport();
void reports();
void backupData();
void restoreData();
void manager();
void clerk();
void user();

struct Account
{
    char name[40];
    char account_no[10];
    char type[10];
    int pin;
    float balance;
};

void main()
{
    int choice;
    while (1)
    {
        printf("\n------ Graphic Era Banking System ------\n");
        printf("1. Manager Mode\n");
        printf("2. Clerk Mode\n");
        printf("3. User Mode\n");
        printf("4. Exit\n");

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

void createAccount()
{
    struct Account acc;
    FILE *fp, *acNumFile;
    int lastAcNo = 18290;

    acNumFile = fopen("accountNumber.txt", "r");
    if (acNumFile != NULL)
    {
        fscanf(acNumFile, "%d", &lastAcNo);
        fclose(acNumFile);
    }

    sprintf(acc.account_no, "%d", lastAcNo); // To pass the value from lastAcNo to acc.account_no

    printf("Enter Account holder's name : ");
    scanf(" %[^\n]", acc.name);
    printf("Enter Account type (Saving/Current/FD/RD): ");
    scanf("%s", acc.type);
    printf("Create a Personal Identification Number(PIN) : ");
    scanf("%d", &acc.pin);
    printf("Enter the amount : ");
    scanf("%f", &acc.balance);

    printf("\nYour Account is created Successfully!\n");

    printf("\nYour Account details: \n");
    printf("Name : %s \n", acc.name);
    printf("Account Number : %s \n", acc.account_no);
    printf("Account Type : %s \n", acc.type);
    printf("PIN : %d \n", acc.pin);
    printf("Balance : %.2f \n", acc.balance);

    writeHistory(acc.account_no, "CREATE", acc.balance); // To store data in history.txt

    fp = fopen("bank.txt", "a");

    if (fp == NULL)
    {
        printf("Error opening bank.txt for writing!\n");
        return;
    }

    fprintf(fp, "%s,%s,%s,%d,%.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
    fclose(fp);

    lastAcNo++;
    acNumFile = fopen("accountNumber.txt", "w");
    fprintf(acNumFile, "%d", lastAcNo);
    fclose(acNumFile);
}

void deposit()
{
    struct Account acc;
    char acNo[10];
    FILE *fp, *temp;
    int found = 0;
    float amount;

    printf("Enter the Account Number: ");
    scanf("%s", acNo);

    printf("Enter the amount to deposit: ");
    scanf("%f", &amount);

    fp = fopen("bank.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
    {
        if (strcmp(acc.account_no, acNo) == 0)
        {
            acc.balance += amount;
            writeHistory(acc.account_no, "DEPOSIT", amount); // To store data in history.txt
            found = 1;
            printf("Deposit successful. New balance: %.2f\n", acc.balance);
        }
        fprintf(temp, "%s,%s,%s,%d,%.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
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

    printf("Enter the Account Number: ");
    scanf("%s", acNo);

    printf("Enter the amount to withdraw: ");
    scanf("%f", &amount);

    fp = fopen("bank.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fp == NULL || temp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
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
            found = 1;
        }
        fprintf(temp, "%s,%s,%s,%d,%.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
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

    printf("Enter Sender's Account Number: ");
    scanf("%s", fromAc);
    printf("Enter Recipient's Account Number: ");
    scanf("%s", toAc);
    printf("Enter amount to transfer: ");
    scanf("%f", &amount);

    fp = fopen("bank.txt", "r");
    temp = fopen("temp.txt", "w");
    if (fp == NULL || temp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
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
            foundTo = 1;
            acc.balance += amount;
            toBalance = acc.balance;
        }
        fprintf(temp, "%s,%s,%s,%d,%.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
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
        printf("Sender's new balance: %.2f\n", fromBalance);
        printf("Recipient's new balance: %.2f\n", toBalance);
        return;
    }
    remove("temp.txt");
}

void accountOperation()
{
    int choice;
    while (1)
    {
        printf("\n1. Deposit Money\n");
        printf("2. Withdraw Money\n");
        printf("3. Transfer Money\n");
        printf("4. Exit\n");

        printf("\nEnter your choice(1,2,3,4) : ");
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
            break;
        default:
            printf("Wrong Choice!\n");
        }
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
    FILE *bankFp = fopen("bank.txt", "r");
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
    printf("Account Holder: %s\n", acc.name);
    printf("Account Number: %s\n", acc.account_no);
    printf("Account Type  : %s\n", acc.type);

    fp = fopen("history.txt", "r");
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

    printf("\nTotal Deposited : %.2f\n", totalDeposit);
    printf("Total Withdrawn : %.2f\n", totalWithdraw);
    printf("Current Balance : %.2f\n", balance);
}

void writeHistory(const char *accNo, const char *operation, float amount)
{
    FILE *fp = fopen("history.txt", "a");
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

    printf("Enter the Account Number : ");
    scanf("%s", accNO);

    fp = fopen("history.txt", "r");
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

    fp = fopen("bank.txt", "r");
    if (fp == NULL)
    {
        printf("No Data found!");
        return;
    }

    printf("\n------ Accounts ------\n");
    printf("%-20s %-10s %-10s %-6s %-10s\n", "Name", "Acc No", "Type", "PIN", "Balance");
    while (fscanf(fp, "%39[^,],%9[^,],%9[^,],%d,%f\n", acc.name, acc.account_no, acc.type, &acc.pin, &acc.balance) == 5)
    {
        printf("%-20s %-10s %-10s %-6d %-10.2f\n", acc.name, acc.account_no, acc.type, acc.pin, acc.balance);
    }
    fclose(fp);
}

void totalBankBalanceReport()
{
    FILE *fp = fopen("bank.txt", "r");
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
    FILE *fp = fopen("history.txt", "r");
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
    printf("Deposits          : %d\n", deposits);
    printf("Withdrawals       : %d\n", withdrawals);
    printf("Total Deposited   : %.2f\n", totalDeposited);
    printf("Total Withdrawn   : %.2f\n", totalWithdrawn);
}

void reports()
{
    int choice;
    while (1)
    {
        printf("\n----- Bank Report Statement -----\n");
        printf("\n1. Account Summary Report\n");
        printf("2. Transaction History Report\n");
        printf("3. Total Bank Balance Report\n");
        printf("4. Daily Summary Report\n");
        printf("5. Exit\n");

        printf("\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            displayAllAccountReport();
            break;
        case 2:
            transactionHistoryReport();
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

void backupData()
{
    FILE *copy, *paste;
    char line[256];

    copy = fopen("bank.txt", "r");
    paste = fopen("bankBackup.txt", "w");
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
    FILE *copy = fopen("bankBackup.txt", "r");
    FILE *paste = fopen("bank.txt", "w");
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
    printf("\nEnter the username: ");
    scanf("%s", username);

    fp = fopen("credentials.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

    fscanf(fp, "%9[^,], %9s", defUsername, defPassword);
    fclose(fp);

    if (strcmp(defUsername, username) != 0)
    {
        printf("Wrong Username!\n");
        return;
    }

    printf("Enter the password: ");
    scanf("%s", password);
    if (strcmp(defPassword, password) != 0)
    {
        printf("Wrong Password!\n");
        return;
    }

    int choice;
    while (1)
    {
        printf("\n------ Graphic Era Banking System ------\n");
        printf("\n1. Create Account\n");
        printf("2. Account Operations\n");
        printf("3. Bank Statement\n");
        printf("4. Generate Bank Reports\n");
        printf("5. Backup\n");
        printf("6. Restore\n");
        printf("7. Exit\n");

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
            reports();
            break;
        case 5:
            backupData();
            break;
        case 6:
            restoreData();
            break;
        case 7:
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
    printf("Enter the username: ");
    scanf("%s", username);

    fp = fopen("credentials.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

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
    if (strcmp(defPassword, password) != 0)
    {
        printf("Wrong Password!\n");
        return;
    }

    int choice;
    while (1)
    {
        printf("\n------ Graphic Era Banking System ------\n");
        printf("\n1. Create Account\n");
        printf("2. Account Operations\n");
        printf("3. Bank Statement\n");
        printf("4. Exit\n");

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
    printf("Enter the username: ");
    scanf("%s", username);

    fp = fopen("credentials.txt", "r");
    if (fp == NULL)
    {
        printf("Error opening file!\n");
        return;
    }

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
    if (strcmp(defPassword, password) != 0)
    {
        printf("Wrong Password!\n");
        return;
    }

    int choice;
    while (1)
    {
        printf("\n------ Graphic Era Banking System ------\n");
        printf("\n1. Create Account\n");
        printf("2. Account Operations\n");
        printf("3. Bank Statement\n");
        printf("4. Exit\n");

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
            printf("Exited...\n");
            return;
        default:
            printf("Wrong Choice!\n");
        }
    }
}