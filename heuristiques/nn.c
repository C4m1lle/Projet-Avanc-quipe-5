#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "../tsp/struct.h"
#include "../bruteforce/demi_matrice.h"
#include "../distance/distance.h"
#include "nn.h"

void plus_proche_voisin(void **tour, DistanceFuncGenerique distance, int *meilleure_tour, double *longueur_totale, int n){
    if (n <= 0) return;

    int *visite = malloc(n * sizeof(int));
    if (visite == NULL) {
        printf("Erreur memoire\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        visite[i] = 0;
    }

    int ville = 0; 
    visite[ville] = 1;
    meilleure_tour[0] = 1; 

    for (int i = 1; i < n; i++) {
        double min;
        int next = -1;
        int first = 1; 

        for (int j = 0; j < n; j++) {
            if (visite[j] == 0) { 
                double d = distance(tour[ville], tour[j]);
                if (first) {
                    min = d;
                    next = j;
                    first = 0;
                }
                else if (d < min) {
                    min = d;
                    next = j;
                }
            }
        }
        meilleure_tour[i] = next + 1;
        visite[next] = 1;
        ville = next;
    }
    *longueur_totale = tour_length(meilleure_tour, distance);
    free(visite);
}

void plus_proche_voisin_demi_matrice(tTournee tour, tDemiMatrice matrice, int *meilleure_tour, double *longueur_totale){
    int n = get_taille_tournee(tour);
    if (n <= 0) return;

    int *visite = malloc(n * sizeof(int));
    if (visite == NULL) {
        printf("Erreur memoire\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        visite[i] = 0;
    }

    int ville = 0;
    visite[ville] = 1;
    meilleure_tour[0] = 1;

    for (int i = 1; i < n; i++) {
        double min;
        int next = -1;
        int first = 1;

        for (int j = 0; j < n; j++) {
            if (visite[j] == 0) {
                double d = obtenir_distance(matrice, ville, j);
                if (first) {
                    min = d;
                    next = j;
                    first = 0;
                }
                else if (d < min) {
                    min = d;
                    next = j;
                }
            }
        }

        meilleure_tour[i] = next + 1;
        visite[next] = 1;
        ville = next;
    }

    *longueur_totale = tour_length(meilleure_tour, (DistanceFunc)obtenir_distance);
    free(visite);
}
