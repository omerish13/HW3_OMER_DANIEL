CC = gcc
OBJS = Airline.o Airport.o AirportManager.o Date.o Flight.o General.o Plane.o list.o main.o
EXEC = demo
DEBUG = -g
CFLAGS = -Wall -Werror $(DEBUG)

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

Airline.o: Airline.c Airline.h Flight.h AirportManager.h Airport.h list.h \
  def.h Date.h Plane.h General.h
Airport.o: Airport.c Airport.h General.h
AirportManager.o: AirportManager.c AirportManager.h Airport.h list.h \
  def.h General.h
Date.o: Date.c General.h Date.h
Flight.o: Flight.c Flight.h AirportManager.h Airport.h list.h def.h \
  Date.h Plane.h General.h
General.o: General.c General.h
Plane.o: Plane.c General.h Plane.h
list.o: list.c list.h def.h
main.o: main.c Airline.h Flight.h AirportManager.h Airport.h list.h def.h \
  Date.h Plane.h General.h



clean:
	rm -f $(OBJS) $(EXEC)