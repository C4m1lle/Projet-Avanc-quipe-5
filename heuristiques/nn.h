/**
 * @file plus_proche_voisin.h
 * @brief Déclarations pour l'heuristique du plus proche voisin (Nearest Neighbor) et 2-opt.
 */

#ifndef __PLUS_PROCHE_VOISIN_H__
#define __PLUS_PROCHE_VOISIN_H__

#include "../tsp/struct.h"
#include "../bruteforce/demi_matrice.h"
#include "../tsp/types.h"
#include "opt2.h"

/**
 * @brief Heuristique du plus proche voisin pour générer une tournée.
 *
 * Cette fonction construit une tournée en choisissant à chaque étape la ville
 * non visitée la plus proche de la ville courante.
 *
 * @param tour Tableau des instances représentant les villes.
 * @param distance Fonction de calcul de distance entre deux instances.
 * @param meilleure_tour Tableau résultat contenant l'ordre des villes (indexés à partir de 1).
 * @param longueur_totale Longueur totale finale de la tournée construite.
 * @param lenght Nombre de villes dans la tournée.
 */
void plus_proche_voisin(void ** tour, DistanceFuncGenerique distance, int *meilleure_tour, double *longueur_totale,int lenght);

/**
 * @brief Version utilisant une demi-matrice pour accélérer le calcul des distances.
 *
 * @param tour Tournée contenant les villes.
 * @param matrice Demi-matrice des distances.
 * @param meilleure_tour Tableau résultat de la tournée (indexés à partir de 1).
 * @param longueur_totale Longueur totale de la tournée.
 */
void plus_proche_voisin_demi_matrice(tTournee tour, tDemiMatrice matrice, int *meilleure_tour, double *longueur_totale);

/**
 * @brief Heuristique plus proche voisin suivie de l'optimisation 2-opt.
 *
 * @param tour Tableau d'instances représentant les villes.
 * @param dist Fonction de distance.
 * @param best Tableau résultat de la meilleure tournée.
 * @param distmin Longueur finale optimisée.
 * @param lenght Nombre de villes.
 */
void deux_optnn(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght);

#endif
