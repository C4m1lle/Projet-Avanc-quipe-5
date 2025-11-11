#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "struct.h"
#include "distance.h"
#include "demi_matrice.h"


void plus_proche_voisin(tTournee tour, DistanceFunc distance, int *meilleure_tour, double *longueur_totale) {
    int n = get_taille_tournee(tour);
    int *visite = malloc(n * sizeof(int));
    int i, j;

    for (i = 0; i < n; i++) {
        visite[i] = 0;
    }

    int ville_actuelle = 0;
    visite[ville_actuelle] = 1;
    meilleure_tour[0] = ville_actuelle;

   
    for (i = 1; i < n; i++) {
        double distance_min = DBL_MAX;
        int prochaine_ville = -1;

        tInstance instance_actuelle = get_instance_at(tour, ville_actuelle);

        for (j = 0; j < n; j++) {
            if (visite[j] == 0) {
                tInstance instance_candidate = get_instance_at(tour, j);
                double d = distance(instance_actuelle, instance_candidate);

                if (d < distance_min) {
                    distance_min = d;
                    prochaine_ville = j;
                }
            }
        }

        meilleure_tour[i] = prochaine_ville;
        visite[prochaine_ville] = 1;
        ville_actuelle = prochaine_ville;
    }

    double somme_distances = 0.0;
    for (i = 0; i < n - 1; i++) {
        tInstance a = get_instance_at(tour, meilleure_tour[i]);
        tInstance b = get_instance_at(tour, meilleure_tour[i + 1]);
        somme_distances += distance(a, b);
    }

    tInstance debut = get_instance_at(tour, meilleure_tour[0]);
    tInstance fin = get_instance_at(tour, meilleure_tour[n - 1]);
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
    meilleure_tour[0] = ville_courante;

    
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
            meilleure_tour[i] = prochaine_ville;
            visite[prochaine_ville] = 1;
            ville_courante = prochaine_ville;
        }
    }

    
    double somme = 0.0;
    for (i = 0; i < n - 1; i++) {
        somme += obtenir_distance(matrice, meilleure_tour[i], meilleure_tour[i + 1]);
    }
    somme += obtenir_distance(matrice, meilleure_tour[n - 1], meilleure_tour[0]); 

    *longueur_totale = somme;

    free(visite);
}

