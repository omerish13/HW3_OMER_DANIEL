#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "AirportManager.h"
#include "General.h"

int	initManager(AirportManager* pManager)
{
	return (int)L_init(pManager->airportsList);
}

int	addAirport(AirportManager* pManager)
{
	Airport* pPort  = (Airport*)calloc(1, sizeof(Airport));
	if (!pPort)
		return 0;

	if (!initAirport(pPort, pManager))
	{
		freeAirport(pPort);
		free(pPort);
		return 0;
	}
	
	insertAirport(pPort,pManager->airportsList);
	return 1;
}

void insertAirport(Airport* pPort, LIST* airportsList)
{
	NODE* pNode = &airportsList->head;
	if (!pNode)
		return;
	Airport* airport=(Airport*)pNode->key, *nextAirport;
	if (strcmp(pPort->code,airport->code))
	{
		L_insert(&airportsList->head,pPort);
		return;
	}
	while (pNode->next)
	{
		airport = (Airport*)pNode->key;
		nextAirport = (Airport*)pNode->next->key;
		if (!airport || !nextAirport)
			return;
		if (strcmp(airport->code,pPort->code) >= 0 && strcmp(pPort->code,nextAirport->code))
		{
			L_insert(pNode,pPort);
			return 1;
		}
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
	NODE* pNode = &pManager->airportsList->head;
	Airport* pPort;	
	while (pNode)
	{
		pPort = (Airport*)pNode->key;
		if (isAirportCode(pPort,code))
			return pPort;
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


void	printAirports(const AirportManager* pManager)
{
	NODE* pNode = &pManager->airportsList->head;
	printf("there are %d airports\n", L_length(pNode));
	while (pNode)
	{
		printAirport(pNode);
		printf("\n");
		pNode = pNode->next;
	}
}

void	freeManager(AirportManager* pManager)
{
	freeAirportArr(pManager);
}


void	freeAirportArr(AirportManager* pManager)
{
	L_free(pManager->airportsList);
}