#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"

// int	initManager(AirportManager* pManager)
// {
// 	return (int)L_init(pManager->airportsList);
// }

int		initManager(AirportManager* pManager, const char* fileName)
{
	
	FILE* fp = fopen(fileName, "r");
	int res = L_init(&pManager->airportsList);
	if (!fp)
	{
		if (res)
		{
			printf("List Initialized");
			return 2;
		}
		return 0;
	}
	int len = 0;
	if (fscanf(fp,"%d\n",&len) != 1)
	{
		fclose(fp);
		if (res)
			return 2;
		return 0;
	}

	NODE* pNode = &pManager->airportsList.head;
	Airport * pPort;
	for (int i = 0; i < len; i++)
	{
		
		pPort = (Airport*)malloc(sizeof(Airport));
		if (!pPort || !readAirportFromTextFile(fp,pPort))
		{
			freeAirportArr(pManager);
			fclose(fp);
			return 2;
		}
		L_insert(pNode,pPort);
		pNode = pNode->next;
	}

	fclose(fp);
	return 1;
	
}
int	addAirport(AirportManager* pManager)
{
	Airport* pPort  = (Airport*)malloc(sizeof(Airport));
	if (!pPort)
		return 0;

	if (!initAirport(pPort, pManager))
	{
		freeAirport(pPort);
		free(pPort);
		return 0;
	}
	
	insertAirport(pPort,&pManager->airportsList);
	return 1;
}

void insertAirport(Airport* pPort, LIST* airportsList)
{
	NODE* pNode = &airportsList->head;
	if (!pNode)
		return;
	
	while (pNode->next && strcmp(pPort->code,((Airport*)(pNode->next->key))->code) >= 0 )
	{
		pNode = pNode->next;
	}
	L_insert(pNode,pPort);
}

int  initAirport(Airport* pPort, AirportManager* pManager)
{

	while (1)
	{
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager))
			break;

		printf("This code already in use - enter a different code\n");
	}
	
	return initAirportNoCode(pPort);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code)
{
	const NODE* pNode = pManager->airportsList.head.next;
	const Airport* pPort;
	while (pNode)
	{
		pPort = (const Airport*)(pNode->key);
		if (pPort)
			if (isAirportCode(pPort,code))
				return pNode->key;
		pNode = pNode->next;
	}
	return NULL;
}

int checkUniqeCode(const char* code,const AirportManager* pManager)
{
	Airport* port = findAirportByCode(pManager, code);
	if (port != NULL)
		return 0;

	return 1;
}

int saveManagerToFile(const AirportManager* pManager, const char* fileName)
{
	const NODE* pNode = pManager->airportsList.head.next;
	FILE* fp;
	fp = fopen(fileName,"w");
	if (!fp)
		return 0;
	
	fprintf(fp,"%d\n",L_length(pNode));
	while (pNode)
	{
		writeAirportToTextFile(fp,(Airport*)pNode->key);
		pNode = pNode->next;
	}

	fclose(fp);
	return 1;
	
	
}

void	printAirports(const AirportManager* pManager)
{
	printf("there are %d airports\n", L_length(pManager->airportsList.head.next));
	L_print(&pManager->airportsList,printAirport);
	printf("\n");
}

void	freeManager(AirportManager* pManager)
{
	freeAirportArr(pManager);
}


void	freeAirportArr(AirportManager* pManager)
{
	L_free(&pManager->airportsList,freeAirport);
}