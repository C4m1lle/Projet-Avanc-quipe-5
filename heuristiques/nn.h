#ifndef __PLUS_PROCHE_VOISIN_H__
#define __PLUS_PROCHE_VOISIN_H__

#include "../tsp/struct.h"
#include "../bruteforce/demi_matrice.h"
#include "../tsp/types.h"

void plus_proche_voisin(void ** tour, DistanceFuncGenerique distance, int *meilleure_tour, double *longueur_totale,int lenght);

void plus_proche_voisin_demi_matrice(tTournee tour, tDemiMatrice matrice, int *meilleure_tour, double *longueur_totale);

#endif

