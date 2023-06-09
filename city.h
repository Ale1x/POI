#ifndef CITY_CITY_H
#define CITY_CITY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SIZE 6
#define M_TRANSPORT 3

typedef struct _road* road;
typedef struct _POI* POI;
typedef struct _city* city;


city create_city(char* city_name);

int create_POI(city _city, char* POI_name);

int create_road(city _city, POI* POI_src,  POI* POI_dest, char* m_transport, int* times, bool one_way);

char* register_trip(city _city, int* path, char* m_transport);

POI crowded_POI(city _city, int* nVisitMAX);

POI ez_reachable(city _city);

//Funzioni aggiuntive utili per l'implementazione del main/menu
POI get_POI(city c, int POI_ID);
int get_POI_index(city _city, POI currentPOI);
char* get_POI_name(POI currentPOI);

#endif //CITY_CITY_H