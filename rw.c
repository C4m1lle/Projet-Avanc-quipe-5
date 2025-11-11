
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
int random_walk(tTournee tour, DistanceFunc dist, int *bestTour, double *bestDist)
{
    if (!tour || !dist || !bestTour || !bestDist)
        return -1;

    int n = get_taille_tournee(tour);
    if (n <= 0)
        return -1;

    // Tableau des villes disponibles
    int *dispo = malloc(sizeof(int) * n);
    if (!dispo) return -1;
    for (int i = 0; i < n; i++)
        dispo[i] = i;

    srand(time(NULL)); // initialisation du générateur aléatoire

    // Crée la tournée aléatoire
    tTournee randomTour = create_tournee(n);
    if (!randomTour) {
        free(dispo);
        return -1;
    }

    int remaining = n; // nombre de villes encore disponibles

    for (int i = 0; i < n; i++) {
        int r;
        if (remaining == 1) {
            r = 0; // si une seule ville restante, on la prend
        } else {
            r = rand() % remaining; // indice aléatoire parmi les restantes
        }

        int chosen_index = dispo[r];
        tInstance inst = get_instance_at(tour, chosen_index);
        add_in_tournee(randomTour, create_instance(get_id(inst), get_x(inst), get_y(inst)));

        // Remplacer l'indice choisi par le dernier élément disponible
        dispo[r] = dispo[remaining - 1];
        remaining--; // réduire le nombre de villes disponibles
    }

    // Calcul de la distance totale (boucle fermée)
    double totalDist = 0.0;
    for (int i = 0; i < n - 1; i++) {
        tInstance a = get_instance_at(randomTour, i);
        tInstance b = get_instance_at(randomTour, i + 1);
        totalDist += dist(a, b);
    }

    // Retour au point de départ
    tInstance last = get_instance_at(randomTour, n - 1);
    tInstance first = get_instance_at(randomTour, 0);
    totalDist += dist(last, first);

    // Stocker les résultats
    *bestDist = totalDist;
    for (int i = 0; i < n; i++)
        bestTour[i] = get_id(get_instance_at(randomTour, i));

    free(dispo);
    delete_tournee(&randomTour);
    return 0;
}




