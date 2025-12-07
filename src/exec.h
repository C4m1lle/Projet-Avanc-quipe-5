/**
 * @file exec.h
 * @brief Exécution d'une méthode de résolution TSP et mesure du temps d'exécution.
 *
 * Fournit une fonction pour lancer une heuristique ou une méthode exacte
 * sur une tournée donnée et calculer le temps nécessaire.
 */

#ifndef __EXEC_H__
#define __EXEC_H__

#include "../tsp/types.h"
#include <time.h>

/**
 * @brief Exécute une méthode de résolution du TSP et mesure son temps d'exécution.
 *
 * Cette fonction appelle la méthode passée en paramètre (solving_method),
 * calcule la distance de la tournée générée et mesure le temps d'exécution
 * en secondes.
 *
 * @param method    Pointeur vers la fonction de résolution (heuristique ou exacte).
 * @param runtime   Pointeur vers un double où stocker le temps d'exécution (en secondes).
 * @param tour      Tableau des instances représentant les villes.
 * @param dist      Fonction de distance entre deux villes (DistanceFuncGenerique).
 * @param best      Tableau d'entiers où stocker la meilleure tournée trouvée.
 * @param distmin   Pointeur vers la distance minimale trouvée.
 * @param lenght    Nombre de villes dans la tournée.
 */
void run(solving_method method, double * runtime, void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght);

#endif
