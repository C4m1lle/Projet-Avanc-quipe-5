#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "nn.h"

/* plus_proche_voisin
 * tour : la tournée contenant les instances
 * distance : fonction de distance
 * meilleure_tour : tableau préalloué pour stocker l'ordre des ids des villes
 */
void plus_proche_voisin(tTournee tour, DistanceFunc distance, int *meilleure_tour)
{
    int n = get_taille_tournee(tour);
    if (n <= 0 || meilleure_tour == NULL) {
        return;
    }

    /* tableau pour savoir quelles villes ont été visitées */
    int *visite = malloc(sizeof(int) * n);
    if (visite == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire\n");
        return;
    }
    int i;
    for (i = 0; i < n; i++) {
        visite[i] = 0;
    }

    /* première ville */
    tInstance ville_courante = get_instance_at(tour, 0);
    meilleure_tour[0] = get_id(ville_courante);
    visite[0] = 1;

    /* étape par étape */
    for (i = 1; i < n; i++) {
        double distance_min = DBL_MAX;
        int prochaine_index = -1;

        int j;
        for (j = 0; j < n; j++) {
            if (visite[j] == 0) {
                tInstance candidate = get_instance_at(tour, j);
                double d = distance(ville_courante, candidate);
                if (d < distance_min) {
                    distance_min = d;
                    prochaine_index = j;
                }
            }
        }

        /* si aucune ville trouvée (robustesse) */
        if (prochaine_index == -1) {
            for (j = 0; j < n; j++) {
                if (visite[j] == 0) {
                    prochaine_index = j;
                    j = n; /* sortir de la boucle */
                }
            }
        }

        if (prochaine_index == -1) {
            /* remplir le reste avec -1 pour signaler l'erreur */
            int k;
            for (k = i; k < n; k++) {
                meilleure_tour[k] = -1;
            }
        } else {
            visite[prochaine_index] = 1;
            ville_courante = get_instance_at(tour, prochaine_index);
            meilleure_tour[i] = get_id(ville_courante);
        }
    }

    free(visite);
}

/* plus_proche_voisin_demi_matrice
 * Variante utilisant une demi-matrice de distances
 */
void plus_proche_voisin_demi_matrice(tTournee tour, tDemiMatrice matrice, int *meilleure_tour)
{
    int n = get_taille_tournee(tour);
    if (n <= 0 || meilleure_tour == NULL || matrice == NULL) {
        return;
    }

    int *visite = malloc(sizeof(int) * n);
    if (visite == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire\n");
        return;
    }
    int i;
    for (i = 0; i < n; i++) {
        visite[i] = 0;
    }

    tInstance ville_courante = get_instance_at(tour, 0);
    meilleure_tour[0] = get_id(ville_courante);
    visite[0] = 1;

    for (i = 1; i < n; i++) {
        double distance_min = DBL_MAX;
        int prochaine_index = -1;
        int id_courante = get_id(ville_courante) - 1; /* 0-based pour matrice */

        int j;
        for (j = 0; j < n; j++) {
            if (visite[j] == 0) {
                tInstance candidate = get_instance_at(tour, j);
                int id_candidate = get_id(candidate) - 1;
                double d = obtenir_distance(matrice, id_courante, id_candidate);
                if (d < distance_min) {
                    distance_min = d;
                    prochaine_index = j;
                }
            }
        }

        if (prochaine_index == -1) {
            for (j = 0; j < n; j++) {
                if (visite[j] == 0) {
                    prochaine_index = j;
                    j = n;
                }
            }
        }

        if (prochaine_index == -1) {
            int k;
            for (k = i; k < n; k++) {
                meilleure_tour[k] = -1;
            }
        } else {
            visite[prochaine_index] = 1;
            ville_courante = get_instance_at(tour, prochaine_index);
            meilleure_tour[i] = get_id(ville_courante);
        }
    }

    free(visite);
}
