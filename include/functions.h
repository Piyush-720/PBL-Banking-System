#ifndef FUNCTIONS_H
#define FUNCTIONS_H

int compareInt(int); // Function to exit in between an ongoing process
int compareFloat(float); // Function to exit in between an ongoing process
int compareString(char *); // Function to exit in between an ongoing process
void createAccount(); // Function to create account
void closeAccount(); // Fuction to close account
void deposit(); // Fuction to deposit money
void withdraw(); // Fuction to withdraw money
void transfer(); // Fuction to transfer money
void accountOperation(); // Main Fuction to perform operations for above 3 functions
void generateBankStatement(); // Fuction to generate Bank Statement
void writeHistory(const char *, const char *, float); // Fuction to write history to history.txt file
void transactionHistoryReport(); // Fuction to show transaction history of a single account on console
void displayAllAccountReport(); // Fuction to show details of all accounts
void totalBankBalanceReport(); // Fuction to display total money in all accounts of bank
void dailyReport(); // Fuction to show daily report such as creation, deposit etc.
void allTransactionHistoryReport(); // Fuction to show transaction history of all account on console
void reports(); // Menu Fuction to call above functions for generating reports
void backupData(); // Fuction to backup data to bankBackup.txt
void restoreData(); // Fuction to restore data to bank.txt
void manager(); // Menu Fuction to call all manager tasks functions
void clerk(); // Menu Fuction to call all clerk tasks functions
void user(); // Menu Fuction to call all user tasks functions
float calculateInterest(const char *accountType, float principal, int days); // Fuction to calculate Interest for different types of accounts
void scheduledInterestUpdate(); // Fuction to calculate interest as soon as the main function runs

#endif