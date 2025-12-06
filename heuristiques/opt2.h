/**
 * @file opt2.h
 * @brief Déclarations des fonctions d'optimisation 2-opt pour le TSP.
 *
 * Fournit des fonctions pour améliorer une tournée existante en réduisant
 * la longueur totale via l'algorithme 2-opt.
 */

#ifndef __OPT_H__
#define __OPT_H__

#include "../tsp/struct.h"
#include "../distance/distance.h"

/**
 * @brief Optimisation 2-opt sur une tournée donnée (first improvement).
 *
 * Cette fonction prend une tournée sous forme de tableau d'instances et
 * tente de réduire sa longueur totale en inversant des segments croisés.
 *
 * @param dist_func Fonction de distance entre deux instances.
 * @param tournee Tableau d'instances représentant la tournée initiale.
 * @param tab_tournee Tableau des indices de la tournée à optimiser.
 * @param lenght Nombre de villes dans la tournée.
 * @return Distance totale après optimisation.
 */
double opt2(DistanceFuncGenerique dist_func,tInstance * tournee, int * tab_tournee, int lenght);

/**
 * @brief Version améliorée de l'optimisation 2-opt (first improvement avec vérification de distance).
 *
 * @param dist_func Fonction de distance entre deux instances.
 * @param tournee Tournée initiale à optimiser.
 * @param tab_tournee Tableau des indices de la tournée à optimiser.
 * @return Distance totale après optimisation.
 */
double opt2_improved(DistanceFuncGenerique dist_func, tTournee tournee, int *tab_tournee);

#endif
