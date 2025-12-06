/**
 * @file bruteforce.h
 * @brief Déclarations des fonctions de résolution du TSP par force brute.
 */

#ifndef __BRUTEFORCE_H__
#define __BRUTEFORCE_H__	

#include "../tsp/types.h"

/**
 * @brief Résout le problème du voyageur de commerce par force brute.
 *
 * Teste toutes les permutations possibles des villes pour déterminer
 * le circuit de distance minimale.
 *
 * @param tour Tableau de pointeurs représentant les villes.
 * @param dist Fonction générique de calcul de distance entre deux villes.
 * @param best Tableau contenant le meilleur parcours trouvé.
 * @param distmin Pointeur vers la distance minimale trouvée.
 * @param lenght Nombre de villes.
 * @param has_to_stop Variable permettant l'arrêt du bruteforce à tout moment par l'implémentation d'un handler.
 */
void bruteforce(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght, int * has_to_stop);

/**
 * @brief Version optimisée de l'algorithme de force brute utilisant une demi-matrice de distances.
 *
 * Cette version évite les recalculs de distance en utilisant une matrice pré-calculée.
 *
 * @param tour Tableau de pointeurs représentant les villes.
 * @param dist Fonction générique de calcul de distance entre deux villes.
 * @param best Tableau contenant le meilleur parcours trouvé.
 * @param distmin Pointeur vers la distance minimale trouvée.
 * @param lenght Nombre de villes.
 * @param has_to_stop Variable permettant l'arrêt du bruteforce à tout moment par l'implémentation d'un handler.
 */
void bruteforce_demi_matrice(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght, int * has_to_stop);

// has_to_stop est une variable permettant l'arrêt du bruteforce à tout moment par l'implémentation d'un handler

#endif
