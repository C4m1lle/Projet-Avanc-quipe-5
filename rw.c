#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rw.h"
#include "struct.h"
#include "distance.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rw.h"
#include "struct.h"
#include "distance.h"

/**
 * Heuristique : Random Walk
 * Crée une tournée aléatoire valide en visitant chaque sommet exactement une fois.
 * 
 * @param tour       tournée de départ (liste des sommets à visiter)
 * @param dist       fonction de distance (eucl2d, att, geo)
 * @param bestTour   tableau d'entiers où stocker l'ordre des IDs des villes
 * @param bestDist   pointeur où stocker la distance totale calculée
 * @return 0 si succès, -1 en cas d'erreur
 */
int random_walk(tTournee tour, DistanceFunc dist, int *bestTour, double *bestDist) {

    if (!tour || !dist || !bestTour || !bestDist)
        return -1;

    int n = get_taille_tournee(tour);
    if (n <= 0)
        return -1;

    // Crée un tableau d’indices [0, 1, 2, ..., n-1]
    int *indices = malloc(sizeof(int) * n);
    if (!indices)
        return -1;

    for (int i = 0; i < n; i++)
        indices[i] = i;

    // Mélange les indices 
    srand(time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = indices[i];
        indices[i] = indices[j];
        indices[j] = tmp;
    }

    // Crée une tournée aléatoire selon l’ordre mélangé
    tTournee randTour = create_tournee(n);
    if (!randTour) {
        free(indices);
        return -1;
    }

    for (int i = 0; i < n; i++) {
        tInstance inst = get_instance_at(tour, indices[i]);
        if (!inst) continue;
        add_in_tournee(randTour, create_instance(get_id(inst), get_x(inst), get_y(inst)));
    }

    // Calcule la distance totale de la tournée
    double total = 0.0;
    for (int i = 0; i < n - 1; i++) {
        tInstance a = get_instance_at(randTour, i);
        tInstance b = get_instance_at(randTour, i + 1);
        total += dist(a, b);
    }

    // Retour au point de départ
    tInstance last = get_instance_at(randTour, n - 1);
    tInstance first = get_instance_at(randTour, 0);
    total += dist(last, first);

    // Sauvegarde les résultats
    *bestDist = total;
    for (int i = 0; i < n; i++) {
        bestTour[i] = get_id(get_instance_at(randTour, i));
    }

    // Libère la mémoire
    free(indices);
    delete_tournee(&randTour);

    return 0;
}




