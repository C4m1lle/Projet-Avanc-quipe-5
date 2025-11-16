#ifndef __RW_H__
#define __RW_H__

#include "../tsp/struct.h"
#include "../bruteforce/demi_matrice.h"

/**
 * random_walk
 * ------------
 * Construit une tournée aléatoire en choisissant à chaque étape un sommet
 * non encore visité de manière aléatoire jusqu’à compléter la tournée.
 *
 * Paramètres :
 *  - problem : le problème TSP chargé via tspReader
 *  - dist    : fonction de distance (DistanceFunc)
 *  - bestTour: pointeur vers un tableau d'entiers où stocker la meilleure tournée
 *  - bestDist: pointeur vers un double où stocker la distance totale
 *
 * Retour :
 *  - 0 si succès
 *  - -1 si erreur
 */
int random_walk(tTournee tour, DistanceFunc dist, int *bestTour, double *bestDist);

#endif
