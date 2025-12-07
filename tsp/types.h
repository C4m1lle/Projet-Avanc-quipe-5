/**
 * @file types.h
 * @brief Déclarations des types génériques utilisés pour les heuristiques et méthodes TSP.
 */

#ifndef __TSP_TYPES_H__
#define __TSP_TYPES_H__

/**
 * @typedef DistanceFuncGenerique
 * @brief Pointeur vers une fonction de calcul de distance entre deux villes.
 * @param a Première ville (void* générique, typiquement tInstance)
 * @param b Deuxième ville (void* générique, typiquement tInstance)
 * @return double Distance entre a et b
 */
typedef double (*DistanceFuncGenerique)(void *, void *);

/**
 * @typedef solving_method
 * @brief Pointeur vers une fonction implémentant une méthode de résolution TSP.
 * @param tournee Tableau des villes à parcourir (void**)
 * @param dist_method Fonction de distance utilisée
 * @param best_path Tableau d'entiers pour stocker la meilleure tournée trouvée
 * @param dist_best_path Pointeur vers le double pour stocker la distance totale
 * @param lenght_tournee Taille de la tournée
 */
typedef void (*solving_method)(void ** tournee, DistanceFuncGenerique dist_method, int * best_path, double * dist_best_path, int lenght_tournee);

#endif
