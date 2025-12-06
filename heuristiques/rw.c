/**
 * @file rw.c
 * @brief Heuristique Random Walk (RW) pour générer une tournée aléatoire pour le TSP.
 *
 * Fournit des fonctions pour créer une tournée aléatoire valide et
 * l'améliorer avec l'optimisation 2-opt.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rw.h"
#include "../tsp/struct.h"
#include "../distance/distance.h"

/**
 * @brief Génère une tournée aléatoire en visitant chaque sommet exactement une fois.
 *
 * La fonction remplit le tableau bestTour avec l'ordre des IDs des villes
 * et calcule la distance totale de la tournée dans bestDist.
 *
 * @param tour       Tableau des instances représentant les villes.
 * @param dist       Fonction de distance entre deux villes (eucl2d, att, geo, etc.).
 * @param bestTour   Tableau d'entiers où stocker l'ordre des IDs des villes.
 * @param bestDist   Pointeur où stocker la distance totale calculée.
 * @param lenght     Nombre de villes dans la tournée.
 */
void random_walk(void ** tour, DistanceFuncGenerique dist, int *bestTour, double *bestDist, int lenght)
{
    int n = lenght;
    if (!tour || !dist || !bestTour || !bestDist || n <= 0){
        fprintf(stderr,"nn : Paramètres invalides\n");
        return;
    }

    // Tableau des villes disponibles
    int *dispo = malloc(sizeof(int) * n);
    if (!dispo) return;
    for (int i = 0; i < n; i++)
        dispo[i] = i;

    srand(time(NULL)); // initialisation du générateur aléatoire

    // Crée la tournée aléatoire
    void ** randomTour = malloc(sizeof(void *)*n);
    int remaining = n; // nombre de villes encore disponibles

    for (int i = 0; i < n; i++) {
        int r;
        if (remaining == 1) {
            r = 0; // si une seule ville restante, on la prend
        } else {
            r = rand() % remaining; // indice aléatoire parmi les restantes
        }

        int chosen_index = dispo[r];
        randomTour[i] = tour[chosen_index];
        bestTour[i] = chosen_index + 1; // génération du tour avec indexation à +1
        dispo[r] = dispo[remaining - 1]; // remplacer l'indice choisi par le dernier disponible
        remaining--;
    }

    // Calcul de la distance totale (boucle fermée)
    double totalDist = 0.0;
    for (int i = 0; i < n - 1; i++) {
        void * a = randomTour[i];
        void * b = randomTour[i+1];
        totalDist += dist(a, b);
    }

    // Retour au point de départ
    void * last = randomTour[n-1];
    void * first = randomTour[0];
    totalDist += dist(last, first);

    // Stocker les résultats
    *bestDist = totalDist;

    free(dispo);
    free(randomTour);
}

/**
 * @brief Heuristique combinée : Random Walk suivi d'une optimisation 2-opt.
 *
 * Génère une tournée aléatoire puis applique l'optimisation 2-opt
 * pour améliorer la distance totale.
 *
 * @param tour    Tableau des instances représentant les villes.
 * @param dist    Fonction de distance entre deux villes.
 * @param best    Tableau d'entiers où stocker la meilleure tournée.
 * @param distmin Pointeur vers la distance minimale trouvée.
 * @param lenght  Nombre de villes dans la tournée.
 */
void deux_optrw(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght){
    random_walk(tour, dist, best, distmin, lenght);
    (*distmin) = opt2(dist, (tInstance *)tour, best, lenght);
}
