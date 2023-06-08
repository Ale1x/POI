#ifndef CITY_CITY_H
#define CITY_CITY_H
 
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
 
typedef struct _road* road;
typedef struct _POI* POI;
typedef struct _city* city;
 
 
city create_city(char* city_name);
 
int create_POI(city _city, char* POI_name);
 
int create_road(city _city, POI* POI_src,  POI* POI_dest, char* m_transport, int* times, bool one_way);
 
char* path_validity(city _city, int* path, char* m_transport);
 
POI crowded_POI(city _city);
 
POI ez_reachable(city _city);
 
POI get_POI(city c, int POI_ID);
 
#endif //CITY_CITY_H