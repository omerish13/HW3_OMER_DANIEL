#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "list.h"

#define FILE_NAME "airport_authority.txt"

typedef struct
{
	LIST*	airportsList;
}AirportManager;

int		initManager(AirportManager* pManager);
int		addAirport(AirportManager* pManager);
int		initAirport(Airport* pPort, AirportManager* pManager);
void     insertAirport(Airport* pPort, LIST* airportsList);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
int		checkUniqeCode(const char* code, const AirportManager* pManager);
int     writeAirportManagerToTextFile(AirportManager* pManager);
void	printAirports(const AirportManager* pManager);
void	freeManager(AirportManager* pManager);
void	freeAirportArr(AirportManager* pManager);
#endif