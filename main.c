#include <stdio.h>
#include <stdlib.h>
#include "Airline.h"
#include "AirportManager.h"
#include "General.h"

typedef enum 
{ 
	eAddAirport,eAddPlane, eAddFlight, ePrintCompany, ePrintAirports,
	ePrintFlightsPlaneType, eSortFlight, eFindFlight, eNofOptions
} eMenuOptions;

const char* str[eNofOptions] = { "Add Airport","Add Plane","Add Flight",
								"Print Airline", "Print all Airports",
								"Print all flights with plane type", "Sort flight by condition", "Find flight by condition" };

#define EXIT			-1
int menu();
int 	initManagerAndAirline(AirportManager* pManager, Airline* pCompany);

int main()
{
	AirportManager	manager;
	Airline			company;

	if (!initManagerAndAirline(&manager,&company))
		return 0;

	int option;
	int stop = 0;

	
	do
	{
		option = menu();
		switch (option)
		{
		case eAddPlane:
			if (!addPlane(&company))
				printf("Error adding plane\n");
			break;

		case eAddAirport:
			if (!addAirport(&manager))
				printf("Error adding airport\n");
			break;

		case eAddFlight:
			if (!addFlight(&company, &manager))
				printf("Error adding flight\n");
			break;

		case ePrintCompany:
			printCompany(&company);
			break;

		case ePrintAirports:
			printAirports(&manager);
			break;

		case ePrintFlightsPlaneType:
			doPrintFlightsWithPlaneType(&company);
			break;
		case eSortFlight:
			sortFlight(&company);
			break;
		case eFindFlight:
			findFlight(&company);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);

	saveManagerToFile(&manager, FILE_NAME);
	saveAirlineToFile(&company, BIN_FILE_NAME);

	freeManager(&manager);
	freeCompany(&company);
	
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < eNofOptions ; i++)
		printf("%d - %s\n",i,str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int 	initManagerAndAirline(AirportManager* pManager, Airline* pCompany)
{
	int res = initManager(pManager,FILE_NAME);

	if (res == 1)
	{
		if (!initAirlineFromFile(pCompany,pManager,BIN_FILE_NAME))
		{
			printf("Initialize from client\n");
			initAirline(pCompany);
		}
		return 1;
	}
	else if (res == 2)
	{
		printf("Can't initialize from file!\n");
		initAirline(pCompany);
		return 1;
	}
	printf("Can't initialize Manager\n");
	return 0;

	// initManager(pManager);
	// initAirline(pCompany);
	// return 1;
	
}