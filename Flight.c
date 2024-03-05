#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"
#include "General.h"


void	initFlight(Flight* pFlight,Plane* thePlane, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->sourceCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	pFlight->flightPlane = *thePlane;
	getCorrectDate(&pFlight->date);
}

int		isFlightFromSourceAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->sourceCode, code) == 0)
		return 1;
		
	return 0;
}


int		isFlightToDestAirport(const Flight* pFlight, const char* code)
{
	if (strcmp(pFlight->destCode, code) == 0)
		return 1;

	return 0;


}

int		isPlaneTypeInFlight(const Flight* pFlight, ePlaneType type)
{
	return (pFlight->flightPlane.type == type);
}

int 	readFlightFromBFile(FILE* fp, Flight* pFlight)
{
	int len;
	if (fread(&len,sizeof(int),1,fp) != 1)
		return 0;
	pFlight->sourceCode = (char*)malloc(len* sizeof(char));
	if (!pFlight->sourceCode)
		return 0;
	if (fread(pFlight->sourceCode,sizeof(char),len,fp) != len)
	{
		free(pFlight->sourceCode);
		return 0;
	}
	if (fread(&len,sizeof(int),1,fp) != 1)
		return 0;
	pFlight->destCode = (char*)malloc(len* sizeof(char));
	if (!pFlight->destCode)
	{
		free(pFlight->sourceCode);
		return 0;
	}
	if (fread(pFlight->destCode,sizeof(char),len,fp) != len)
	{
		free(pFlight->sourceCode);
		free(pFlight->destCode);
		return 0;
	}
	if (fread(&pFlight->flightPlane.serialNum,sizeof(int),1,fp) != 1)
	{
		free(pFlight->sourceCode);
		free(pFlight->destCode);
		return 0;
	}
	if (fread(&pFlight->date.day,sizeof(int),1,fp) != 1)
	{
		free(pFlight->sourceCode);
		free(pFlight->destCode);
		return 0;
	}
	if (fread(&pFlight->date.month,sizeof(int),1,fp) != 1)
	{
		free(pFlight->sourceCode);
		free(pFlight->destCode);
		return 0;
	}
	if (fread(&pFlight->date.year,sizeof(int),1,fp) != 1)
	{
		free(pFlight->sourceCode);
		free(pFlight->destCode);
		return 0;
	}
	return 1;
}
int     writeFlightToBinFile(FILE* fp, const Flight* pFlight)
{
	int len = (int)strlen(pFlight->sourceCode);
	if (fwrite(&len,sizeof(int),1,fp) != 1)
		return 0;
	if (fwrite(pFlight->sourceCode,sizeof(char*),1,fp) != 1)
		return 0;
	
	len = (int)strlen(pFlight->destCode);
	if (fwrite(&len,sizeof(int),1,fp) != 1)
		return 0;
	if (fwrite(pFlight->destCode,sizeof(char*),1,fp) != 1)
		return 0;

	if (fwrite(pFlight->flightPlane.serialNum,sizeof(int),1,fp) != 1)
		return 0;
		
	if (fwrite(pFlight->date.day,sizeof(int),1,fp) != 1)
		return 0;
	if (fwrite(pFlight->date.month,sizeof(int),1,fp) != 1)
		return 0;
	if (fwrite(pFlight->date.year,sizeof(int),1,fp) != 1)
		return 0;
}

void	printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s\t",pFlight->sourceCode, pFlight->destCode);
	printDate(&pFlight->date);
	printf("\t");
	printPlane(&pFlight->flightPlane);
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		getAirportCode(code);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport with this code - try again\n");
	} while (port == NULL);

	return port;
}
