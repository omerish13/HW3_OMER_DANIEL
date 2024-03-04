#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"

typedef enum { sortBySourceCode, sortByDestCode, sortByDate, notSorted, numOfSorts } enumSort;
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
int		addFlight(Airline* pComp,const AirportManager* pManager);
int		addPlane(Airline* pComp);
Plane*	FindAPlane(Airline* pComp);
int		compareBySourceCode(const void *f1, const void *f2);
int		compareByDestCode(const void *f1, const void *f2);
int		compareByDate(const void *f1, const void *f2);
void    sortFlights(Airline* company);
void	printCompany(const Airline* pComp);
void	printFlightArr(Flight** arr, int size);
void	printPlanesArr(Plane* arr,int size);
void	doPrintFlightsWithPlaneType(const Airline* pComp);
void	freeFlightArr(Flight** arr, int size);
void	freePlanes(Plane* arr, int size);
void	freeCompany(Airline* pComp);

#endif

