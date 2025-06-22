#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int compareInt(int);
int compareFloat(float);
int compareString(char *);
void createAccount();
void closeAccount();
void deposit();
void withdraw();
void transfer();
void accountOperation();
void generateBankStatement();
void writeHistory(const char *, const char *, float);
void transactionHistoryReport();
void displayAllAccountReport();
void totalBankBalanceReport();
void dailyReport();
void allTransactionHistoryReport();
void reports();
void backupData();
void restoreData();
void manager();
void clerk();
void user();
float calculateInterest(const char *accountType, float principal, int days);
void scheduledInterestUpdate();

#endif