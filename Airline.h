#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"

#define BIN_FILE_NAME "airline.bin"

typedef enum { sortBySourceCode, sortByDestCode, sortByDate, notSorted, numOfSorts } enumSort;
const char* sortOptions[numOfSorts] = { "Sort by source code","Sort by dest code","Sort by date"};

typedef struct
{
	char*		name;
	int			flightCount;
	Flight**	flightArr;
	int			planeCount;
	Plane*		planeArr;
	enumSort	sortType;
}Airline;

void	initAirline(Airline* pComp);
int 	initAirlineFromFile(Airline* pComp, AirportManager* pManager, const char* fileName);
int 	initManagerAndAirline(AirportManager* pManager, Airline* pCompany);
int		addFlight(Airline* pComp,const AirportManager* pManager);
int		addPlane(Airline* pComp);
Plane*	FindAPlane(Airline* pComp);
int		compareBySourceCode(const void *f1, const void *f2);
int		compareByDestCode(const void *f1, const void *f2);
int		compareByDate(const void *f1, const void *f2);
int    	sortFlights(Airline* pComp);
void 	sortFlight(Airline* pComp);
Flight** findFlightBSearch(const Airline* pComp, const Flight* pFlight);
void 	findFlight(const Airline* pComp);
int     saveAirlineToFile(const Airline* pComp, const char* fileName);
void	printCompany(const Airline* pComp);
void	printFlightArr(Flight** arr, int size);
void	printPlanesArr(Plane* arr,int size);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freePlanes(Plane* arr, int size);
void	freeCompany(Airline* pComp);

#endif

