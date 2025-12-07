/**
 * @file rw.h
 * @brief Heuristique Random Walk (RW) pour le TSP.
 *
 * Fournit des fonctions pour générer une tournée aléatoire et
 * l'améliorer avec l'optimisation 2-opt.
 */

#ifndef __RW_H__
#define __RW_H__

#include "../tsp/struct.h"
#include "../bruteforce/demi_matrice.h"
#include "../tsp/types.h"
#include "opt2.h"

/**
 * @brief Génère une tournée aléatoire.
 *
 * La fonction construit une tournée en choisissant à chaque étape
 * un sommet non encore visité de manière aléatoire, puis calcule
 * sa distance totale.
 *
 * @param tour       Tableau des instances représentant les villes.
 * @param dist       Fonction de distance entre deux villes (DistanceFuncGenerique).
 * @param bestTour   Tableau d'entiers où stocker l'ordre des IDs des villes.
 * @param bestDist   Pointeur vers un double où stocker la distance totale.
 * @param lenght     Nombre de villes dans la tournée.
 */
void random_walk(void ** tour, DistanceFuncGenerique dist, int *bestTour, double *bestDist,int lenght);

/**
 * @brief Heuristique combinée Random Walk + 2-opt.
 *
 * Génère une tournée aléatoire via random_walk puis applique l'optimisation 2-opt
 * pour améliorer la distance totale de la tournée.
 *
 * @param tour       Tableau des instances représentant les villes.
 * @param dist       Fonction de distance entre deux villes.
 * @param best       Tableau d'entiers où stocker la meilleure tournée.
 * @param distmin    Pointeur vers la distance minimale trouvée.
 * @param lenght     Nombre de villes dans la tournée.
 */
void deux_optrw(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght);

#endif
