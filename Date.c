#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "General.h"
#include "Date.h"

const int DAY_MONTHS[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
#define SPECIAL_TAV '#'

void getCorrectDate(Date* pDate)
{
	char date[MAX_STR_LEN];
	int ok = 1;

	do {
		printf("Enter Flight Date dd%c%cmm%c%cyyyy  minimum year %d\t",
			SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, SPECIAL_TAV, MIN_YEAR);
		myGets(date, MAX_STR_LEN);
		ok = checkDate(date, pDate);
		if (!ok)
			printf("Error try again\n");
	} while (!ok);
}


int	 checkDate(char* date, Date* pDate)
{
	int day, month, year;
	if (strlen(date) != 12)
		return 0;
	if ( (date[2] != SPECIAL_TAV) || (date[3] != SPECIAL_TAV)
		 || (date[6] != SPECIAL_TAV) || (date[7] != SPECIAL_TAV))
		return 0;
	sscanf(date, "%d%*c%*c%d%*c%*c%d", &day, &month, &year);
	if (day < 1 || month < 1 || month > 12 || year < MIN_YEAR)
		return 0;

	if (day > DAY_MONTHS[month - 1])
		return 0;

	pDate->day = day;
	pDate->month = month;
	pDate->year = year;

	return 1;
}

int     compareDate(const Date d1, const Date d2)
{
	if (d1.year > d2.year)
		return 1;
	else if (d1.year == d2.year)
	{
		if (d1.month > d2.month)
			return 1;
		else if (d1.month == d2.month)
		{
			if (d1.day > d2.day)
			{
				return 1;
			}
			else if (d1.day == d2.day)
				return 0;
		}
	}
	return -1;
}

void printDate(const Date* pDate)
{
	printf("Date: %d/%d/%d", pDate->day, pDate->month, pDate->year);
}
