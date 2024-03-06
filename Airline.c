#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Airline.h"
#include "Airport.h"
#include "General.h"

const char* sortOptions[numOfSorts - 1] = { "Sort by source code","Sort by dest code","Sort by date"};

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

int initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName)
{
	FILE* fp = fopen(fileName,"rb");
	if (!fp)
		return 0;
	int len;
	if (fread(&len,sizeof(int),1,fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	pComp->name = (char*)malloc(len * sizeof(char));
	if (!pComp->name)
	{
		fclose(fp);
		return 0;
	}
	if (fread(pComp->name,sizeof(char*),1,fp) != len)
	{
		free(pComp->name);
		fclose(fp);
		return 0;
	}

	if (fread(&pComp->planeCount,sizeof(int),1,fp) != 1)
	{
		free(pComp->name);
		fclose(fp);
		return 0;
	}
	pComp->planeArr = (Plane*)realloc(pComp->planeArr,pComp->planeCount * sizeof(Plane));
	if (!pComp->planeArr)
	{
		free(pComp->name);
		fclose(fp);
		return 0;
	}
	for (int i = 0; i < pComp->planeCount; i++)
	{
		if (!readPlaneFromBFile(fp,&pComp->planeArr[i]))
		{
			free(pComp->name);
			freePlanes(pComp->planeArr,i);
			fclose(fp);
			return 0;
		}
	}

	if (fread(&pComp->flightCount,sizeof(int),1,fp) != 1)
	{
		free(pComp->name);
		freePlanes(pComp->planeArr,pComp->planeCount);
		fclose(fp);
		return 0;
	}
	pComp->flightArr = (Flight**)realloc(pComp->flightArr,pComp->planeCount * sizeof(Flight*));
	if (!pComp->flightArr)
	{
		free(pComp->name);
		freePlanes(pComp->planeArr,pComp->planeCount);
		fclose(fp);
		return 0;
	}
	
	for (int i = 0; i < pComp->flightCount; i++)
	{
		if (!readFlightFromBFile(fp,pComp->flightArr[i]))
		{
			free(pComp->name);
			freePlanes(pComp->planeArr,pComp->planeCount);
			freeFlightArr(pComp->flightArr,i);
			fclose(fp);
			return 0;
		}
	}

	return 1;
}

int 	initManagerAndAirline(AirportManager* pManager, Airline* pCompany)
{
	int res = initManager(pManager,FILE_NAME);
	if (res == 1)
	{
		if (!initAirlineFromFile(pCompany,pManager,BIN_FILE_NAME))
		{
			initAirline(pCompany);
		}
		return 1;
	}
	else if (res == 2)
	{
		printf("Can't initialize from file!");
	}
	return 0;
	
}

int	addFlight(Airline* pComp,const AirportManager* pManager)
{
	if (L_length(&pManager->airportsList->head) < 2)
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

int    sortFlights(Airline* pComp)
{
	if (pComp->flightCount < 2)
		return 0;
	
	if ((int)(pComp->sortType) == 0)
		qsort(pComp->flightArr,pComp->flightCount,sizeof(Flight),compareBySourceCode);
	else if ((int)(pComp->sortType) == 1)
		qsort(pComp->flightArr,pComp->flightCount,sizeof(Flight),compareByDestCode);
	else if ((int)(pComp->sortType) == 2)
		qsort(pComp->flightArr,pComp->flightCount,sizeof(Flight),compareByDate);
	return 1;
	
}

void 	sortFlight(Airline* pComp)
{
	
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for(int i = 0 ; i < numOfSorts - 2 ; i++)
		printf("%d - %s\n",i,sortOptions[i]);
	scanf("%d", &pComp->sortType);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	if (!sortFlights(pComp))
		printf("Airline company has less then 2 flights, no need to sort!");

}

Flight** findFlightBSearch(const Airline* pComp, const Flight* pFlight)
{
	if (pComp->flightCount < 1 || pComp->sortType == numOfSorts - 1)
		return NULL;
	Flight** pFound = NULL;
	if ((int)(pComp->sortType) == 0)
		pFound = (Flight**)bsearch(pFlight,pComp->flightArr,pComp->flightCount,sizeof(Flight*),compareBySourceCode);
	else if ((int)(pComp->sortType) == 1)
		pFound = (Flight**)bsearch(pFlight,pComp->flightArr,pComp->flightCount,sizeof(Flight*),compareByDestCode);
	else if ((int)(pComp->sortType) == 2)
		pFound = (Flight**)bsearch(pFlight,pComp->flightArr,pComp->flightCount,sizeof(Flight*),compareByDate);
	return pFound;
}

void 	findFlight(const Airline* pComp)
{
	Flight* temp = (Flight*)malloc(sizeof(Flight));
	if (!temp)
	{
		printf("Memory allocation for flight failed!");
		return;
	}
	switch (pComp->sortType)
	{
	case notSorted:
		printf("Flights array not sorted, please sort it first");
		return;
	case sortByDate:
		getCorrectDate(&temp->date);
		break;
	case sortBySourceCode:
		getAirportCode(temp->sourceCode);
		break;
	case sortByDestCode:
		getAirportCode(temp->destCode);
		break;
	default:
		break;
	}
	Flight** res = findFlightBSearch(pComp,temp);
	printFlight((Flight*)res);

}

int     saveAirlineToFile(const Airline* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName,"wb");
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
	
	if (fwrite(&pComp->planeCount,sizeof(int),1,fp) != 1)
	{
		fclose(fp);
		return 0;
	}
	
	if (fwrite(&pComp->planeCount,sizeof(Plane),1,fp) != 1)
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

	if (fwrite(&pComp->flightCount,sizeof(int),1,fp) != 1)
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

	return 1;
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
