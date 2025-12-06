/**
 * @file distance.h
 * @brief Déclarations des fonctions de calcul de distances entre instances pour différents types de TSP.
 */

#ifndef __DISTANCE_H__
#define __DISTANCE_H__

#include "../tsp/struct.h"
#include "../tsp/types.h"

/**
 * @typedef DistanceFunc
 * @brief Type de fonction de distance entre deux instances.
 */
typedef double (*DistanceFunc)(tInstance, tInstance);

/**
 * @brief Calcule la distance euclidienne 2D entre deux instances.
 *
 * @param a Première instance.
 * @param b Deuxième instance.
 * @return Distance euclidienne arrondie à l'entier le plus proche.
 */
double dist_eucl2d(tInstance a, tInstance b);

/**
 * @brief Calcule la distance pseudo-euclidienne (ATT) entre deux instances.
 *
 * @param a Première instance.
 * @param b Deuxième instance.
 * @return Distance pseudo-euclidienne arrondie selon la norme ATT.
 */
double dist_att(tInstance a, tInstance b);

/**
 * @brief Calcule la distance géographique (GEO) entre deux instances.
 *
 * @param a Première instance.
 * @param b Deuxième instance.
 * @return Distance géographique entre les deux instances.
 */
double dist_geo(tInstance a, tInstance b);

/**
 * @brief Calcule la longueur totale d'une tournée avec une fonction de distance donnée.
 *
 * @param tour Tournée à mesurer.
 * @param dist Fonction de calcul de distance entre deux instances.
 * @return Longueur totale de la tournée.
 */
double tour_length(tTournee tour, DistanceFuncGenerique dist);

/**
 * @brief Initialise une tournée canonique et calcule sa distance totale.
 *
 * @param tour Tableau de pointeurs vers les instances.
 * @param dist Fonction de calcul de distance.
 * @param best Tableau qui sera rempli avec l'ordre des indices de la tournée.
 * @param distmin Pointeur vers la distance totale de la tournée.
 * @param lenght Nombre de villes dans la tournée.
 */
void canonical(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght);

#endif
