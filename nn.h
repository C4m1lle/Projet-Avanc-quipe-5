#ifndef __NN_H__
#define __NN_H__

#include "struct.h"
#include "distance.h"
#include "demi_matrice.h"

/* Algorithme du plus proche voisin */
void plus_proche_voisin(tTournee tour, DistanceFunc distance, int *meilleure_tour);

/* Variante utilisant une demi-matrice de distances */
void plus_proche_voisin_demi_matrice(tTournee tour, tDemiMatrice matrice, int *meilleure_tour);

#endif
