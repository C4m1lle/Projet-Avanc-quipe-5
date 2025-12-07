/**
 * @file exec.c
 * @brief Exécution d'une heuristique ou méthode de résolution TSP et mesure du temps.
 */

#include "exec.h"
#include <time.h>

/**
 * @brief Exécute une méthode de résolution du TSP et mesure son temps d'exécution.
 *
 * La fonction appelle la méthode passée en paramètre, calcule la distance
 * de la tournée générée et mesure le temps d'exécution en secondes.
 *
 * @param method    Pointeur vers la fonction de résolution (ex: plus_proche_voisin, random_walk, etc.).
 * @param runtime   Pointeur vers un double où stocker le temps d'exécution (en secondes).
 * @param tour      Tableau des instances représentant les villes.
 * @param dist      Fonction de distance entre deux villes.
 * @param best      Tableau d'entiers où stocker la meilleure tournée trouvée.
 * @param distmin   Pointeur vers la distance minimale trouvée.
 * @param lenght    Nombre de villes dans la tournée.
 */
void run(solving_method method, double * runtime, void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght){
    clock_t start = clock();
    method(tour, dist, best, distmin, lenght);
    clock_t end = clock();
    (*runtime) = (double)(end - start) / CLOCKS_PER_SEC;
}
