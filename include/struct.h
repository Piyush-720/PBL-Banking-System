#ifndef STRUCT_H
#define STRUCT_H

struct Account
{
	char name[40];
	char account_no[10];
	char type[10];
	int pin;
	float balance;
	char open_date[11]; // YYYY-MM-DD
};

#endif