#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"

void	initAirline(Airline* pComp)
{
	//printf("-----------  Init Airline\n");
	pComp->name = getStrExactName("Enter Airline name");
	pComp->flightArr = NULL;
	pComp->flightCount = 0;
	pComp->planeArr = NULL;
	pComp->planeCount = 0;
	pComp->sortType = notSorted;
}

int	addFlight(Airline* pComp,const AirportManager* pManager)
{
	if (L_length(pManager->airportsList) < 2)
	{
		printf("There are not enough airport to set a flight\n");
		return 0;
	}
	if(pComp->planeCount == 0)
	{
		printf("There is no plane in company\n");
		return 0;
	}

	Flight* pFlight = (Flight*)calloc(1, sizeof(Flight));
	if (!pFlight)
		return 0;
	
	Plane* thePlane = FindAPlane(pComp);
	printAirports(pManager);
	initFlight(pFlight, thePlane,pManager);

	pComp->flightArr = (Flight**)realloc(pComp->flightArr, (pComp->flightCount + 1) * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pFlight);
		return 0;
	}
	pComp->flightArr[pComp->flightCount] = pFlight;
	pComp->flightCount++;
	return 1;
}

int		addPlane(Airline* pComp)
{
	pComp->planeArr = (Plane*)realloc(pComp->planeArr, (pComp->planeCount + 1) * sizeof(Plane));
	if (!pComp->planeArr)
		return 0;
	initPlane(&pComp->planeArr[pComp->planeCount], pComp->planeArr, pComp->planeCount);
	pComp->planeCount++;
	return 1;
}

Plane* FindAPlane(Airline* pComp)
{
	printf("Choose a plane from list, type its serial Number\n");
	printPlanesArr(pComp->planeArr,pComp->planeCount);
	int sn;
	Plane* temp = NULL;
	do {
		scanf("%d", &sn);
		temp = findPlaneBySN(pComp->planeArr,pComp->planeCount, sn);
		if (!temp)
			printf("No plane with that serial number! Try again!\n");
	} while (temp == NULL);
	 
	return temp;
}

int		compareBySourceCode(const void *f1, const void *f2)
{
	char* ch_f1 = *((char**)((Flight*)f1)->sourceCode);
	char* ch_f2 = *((char**)((Flight*)f2)->sourceCode);

	return strcmp(ch_f1,ch_f2);
}

int		compareByDestCode(const void *f1, const void *f2)
{
	char* ch_f1 = *((char**)((Flight*)f1)->destCode);
	char* ch_f2 = *((char**)((Flight*)f2)->destCode);

	return strcmp(ch_f1,ch_f2);
}

int		compareByDate(const void *f1, const void *f2)
{
	Date d_f1 = ((Flight*)f1)->date;
	Date d_f2 = ((Flight*)f2)->date;

	return compareDate(d_f1,d_f2);
}

void    sortFlights(Airline* pComp)
{
	if (pComp->flightCount < 2)
		return;
	
	if ((int)(pComp->sortType) == 0)
		qsort(pComp->flightArr,pComp->flightCount,sizeof(Flight),compareBySourceCode);
	else if ((int)(pComp->sortType) == 1)
		qsort(pComp->flightArr,pComp->flightCount,sizeof(Flight),compareByDestCode);
	else if ((int)(pComp->sortType) == 2)
		qsort(pComp->flightArr,pComp->flightCount,sizeof(Flight),compareByDate);
	return;
	
}

Flight* findFlight(const Airline* pComp, const Flight* pFlight)
{
	if (pComp->flightArr < 1 || pComp->sortType == numOfSorts - 1)
		return NULL;
	Flight* pFound = NULL;
	if ((int)(pComp->sortType) == 0)
		pFound = (Flight*)bsearch(pFlight,pComp->flightArr,pComp->flightCount,sizeof(Flight),compareBySourceCode);
	else if ((int)(pComp->sortType) == 1)
		pFound = (Flight*)bsearch(pFlight,pComp->flightArr,pComp->flightCount,sizeof(Flight),compareByDestCode);
	else if ((int)(pComp->sortType) == 2)
		pFound = (Flight*)bsearch(pFlight,pComp->flightArr,pComp->flightCount,sizeof(Flight),compareByDate);
	return pFound;
}

int     writeToBinFile(const Airline* pComp)
{
	FILE* fp;
	fp = fopen(BIN_FILE_NAME,"wb");
	if (!fp)
		return 0;

	int len = (int)strlen(pComp->name)+1;
	if (fwrite(&len,sizeof(int),1,fp) != 1)
	{
		fclose(fp);
		return 0;
	}
		
	if (fwrite(pComp->name,sizeof(char*),1,fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	
	if (fwrite(pComp->planeCount,sizeof(int),1,fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	
	if (fwrite(pComp->planeCount,sizeof(Plane),1,fp) != 1)
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pComp->planeCount; i++)
	{
		if (!writePlaneToBinFile(fp,&pComp->planeArr[i]))
		{
			fclose(fp);
			return 0;
		}
	}

	if (fwrite(pComp->flightCount,sizeof(int),1,fp) != 1)
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (!writeFlightToBinFile(fp,pComp->flightArr[i]))
		{
			fclose(fp);
			return 0;
		}
	}
}

void printCompany(const Airline* pComp)
{
	printf("Airline %s\n", pComp->name);
	printf("\n -------- Has %d planes\n", pComp->planeCount);
	printPlanesArr(pComp->planeArr, pComp->planeCount);
	printf("\n\n -------- Has %d flights\n", pComp->flightCount);
	printFlightArr(pComp->flightArr, pComp->flightCount);
}

void	printFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		printFlight(arr[i]);
}

void	printPlanesArr(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		printPlane(&arr[i]);
}

void	doPrintFlightsWithPlaneType(const Airline* pComp)
{
	ePlaneType type = getPlaneType();
	int count = 0;
	printf("Flights with plane type %s:\n", GetPlaneTypeStr(type));
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (isPlaneTypeInFlight(pComp->flightArr[i], type))
		{
			printFlight(pComp->flightArr[i]);
			count++;
		}
	}
	if(count == 0)
		printf("Sorry - could not find a flight with plane type %s:\n", GetPlaneTypeStr(type));
	printf("\n");
}

void	freeFlightArr(Flight** arr, int size)
{
	for (int i = 0; i < size; i++)
		free(arr[i]);
}

void	freePlanes(Plane* arr, int size)
{
	for (int i = 0; i < size; i++)
		freePlane(&arr[i]);
}

void	freeCompany(Airline* pComp)
{
	freeFlightArr(pComp->flightArr, pComp->flightCount);
	free(pComp->flightArr);
	free(pComp->planeArr);
	free(pComp->name);
}
