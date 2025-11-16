#ifndef __PLUS_PROCHE_VOISIN_H__
#define __PLUS_PROCHE_VOISIN_H__

#include "../tsp/struct.h"
#include "../bruteforce/demi_matrice.h"

void plus_proche_voisin(tTournee tour, DistanceFunc distance, int *meilleure_tour, double *longueur_totale);

void plus_proche_voisin_demi_matrice(tTournee tour, tDemiMatrice matrice, int *meilleure_tour, double *longueur_totale);

#endif

