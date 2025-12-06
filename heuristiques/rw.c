
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rw.h"
#include "../tsp/struct.h"
#include "../distance/distance.h"

/**
 * Heuristique : Random Walk
 * Crée une tournée aléatoire valide en visitant chaque sommet exactement une fois.
 * 
 * @param tour       tournée de départ (liste des sommets à visiter)
 * @param dist       fonction de distance (eucl2d, att, geo)
 * @param bestTour   tableau d'entiers où stocker l'ordre des IDs des villes
 * @param bestDist   pointeur où stocker la distance totale calculée
 * @param lenght     Longueur de la tournée
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
        bestTour[i] = chosen_index + 1;//génération du tour avec début de l'indexation à +1 par convention du Ttournee
        // Remplacer l'indice choisi par le dernier élément disponible
        dispo[r] = dispo[remaining - 1];
        remaining--; // réduire le nombre de villes disponibles
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
    return;
}



void deux_optrw(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght){
    random_walk(tour, dist, best, distmin,lenght);
    (*distmin) = opt2(dist,(tInstance *)tour,best,lenght);
}
