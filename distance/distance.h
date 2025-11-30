#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include "../tsp/struct.h"
#include "../tsp/types.h"

// Type de fonction de distance
typedef double (*DistanceFunc)(tInstance, tInstance);

// Fonctions de distance
double dist_eucl2d(tInstance a, tInstance b);
double dist_att(tInstance a, tInstance b);
double dist_geo(tInstance a, tInstance b);

// Fonction de calcul de la longueur d’une tournée
double tour_length(tTournee tour, DistanceFunc dist);
void canonical(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght);

#endif

