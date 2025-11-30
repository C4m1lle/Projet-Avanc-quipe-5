#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "../tsp/struct.h"
#include "../bruteforce/demi_matrice.h"
#include "../distance/distance.h"
#include "nn.h"

void plus_proche_voisin(void ** tour, DistanceFuncGenerique distance, int *meilleure_tour, double *longueur_totale,int lenght)
{
    int n = lenght;
    if (n <= 0 || meilleure_tour == NULL) {
        return;
    }

    int *visite = malloc(n * sizeof(int));
    if (visite == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire\n");
        return;
    }

    int i, j;
    for (i = 0; i < n; i++) {
        visite[i] = 0;
    }

    int ville_courante = 0; 
    visite[ville_courante] = 1;
    meilleure_tour[0] = ville_courante + 1; 

   
    for (i = 1; i < n; i++) {
        double distance_min = DBL_MAX;
        int prochaine_ville = -1;

        void * instance_actuelle = tour[ville_courante];

        for (j = 0; j < n; j++) {
            if (visite[j] == 0) {
                void * instance_cand = tour[j];
                double d = distance(instance_actuelle, instance_cand);
                if (d < distance_min) {
                    distance_min = d;
                    prochaine_ville = j;
                }
            }
        }

        if (prochaine_ville == -1) {
            for (j = i; j < n; j++) {
                meilleure_tour[j] = -1;
            }
            i = n;
        } else {
            meilleure_tour[i] = prochaine_ville + 1; 
            visite[prochaine_ville] = 1;
            ville_courante = prochaine_ville;
        }
    }

    
    double somme_distances = 0.0;
    for (i = 0; i < n - 1; i++) {
        void * a = tour[meilleure_tour[i]-1];
        void * b = tour[meilleure_tour[i + 1]-1];
        somme_distances += distance(a, b);
    }

    
    void * debut = tour[meilleure_tour[0]-1];
    void * fin = tour[meilleure_tour[n - 1]-1];
    somme_distances += distance(fin, debut);

    *longueur_totale = somme_distances;

    free(visite);
}

void plus_proche_voisin_demi_matrice(tTournee tour, tDemiMatrice matrice, int *meilleure_tour, double *longueur_totale)
{
    int n = get_taille_tournee(tour);
    if (n <= 0 || meilleure_tour == NULL || matrice == NULL) {
        return;
    }

    int *visite = malloc(n * sizeof(int));
    if (visite == NULL) {
        fprintf(stderr, "Erreur : allocation mémoire\n");
        return;
    }

    int i, j;
    for (i = 0; i < n; i++) {
        visite[i] = 0;
    }

    int ville_courante = 0; 
    visite[ville_courante] = 1;
    meilleure_tour[0] = ville_courante + 1; 

    
    for (i = 1; i < n; i++) {
        double distance_min = DBL_MAX;
        int prochaine_ville = -1;

        for (j = 0; j < n; j++) {
            if (visite[j] == 0) {
                double d = obtenir_distance(matrice, ville_courante, j);
                if (d < distance_min) {
                    distance_min = d;
                    prochaine_ville = j;
                }
            }
        }

        if (prochaine_ville == -1) {
            for (j = i; j < n; j++) {
                meilleure_tour[j] = -1;
            }
            i = n;
        } else {
            meilleure_tour[i] = prochaine_ville + 1; 
            visite[prochaine_ville] = 1;
            ville_courante = prochaine_ville;
        }
    }


    double somme = 0.0;
    for (i = 0; i < n - 1; i++) {
        int a = meilleure_tour[i] - 1;
        int b = meilleure_tour[i + 1] - 1;
        somme += obtenir_distance(matrice, a, b);
    }
    somme += obtenir_distance(matrice, meilleure_tour[n - 1] - 1, meilleure_tour[0] - 1);

    *longueur_totale = somme;

    free(visite);
}

