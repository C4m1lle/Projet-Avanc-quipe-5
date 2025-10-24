#include <stdlib.h>
#include <stdio.h>
#include "demi_matrice.h"

// Création d'une demi-matrice vide
DemiMatrice *creer_demi_matrice(int nb_villes) {
    DemiMatrice *matrice = malloc(sizeof(DemiMatrice));
    if (!matrice) return NULL;

    matrice->nb_villes = nb_villes;
    matrice->lignes = malloc(nb_villes * sizeof(double*));
    if (!matrice->lignes) { free(matrice); return NULL; }

    for (int i = 0; i < nb_villes - 1; i++) {
        matrice->lignes[i] = malloc((nb_villes - i - 1) * sizeof(double));
        if (!matrice->lignes[i]) {
            for (int k = 0; k < i; k++) free(matrice->lignes[k]);
            free(matrice->lignes);
            free(matrice);
            return NULL;
        }
    }
    matrice->lignes[nb_villes - 1] = NULL;
    return matrice;
}

// Destruction
void detruire_demi_matrice(DemiMatrice *matrice) {
    if (!matrice) return;
    for (int i = 0; i < matrice->nb_villes - 1; i++) free(matrice->lignes[i]);
    free(matrice->lignes);
    free(matrice);
}

// Accès / définition
double obtenir_distance(const DemiMatrice *matrice, int i, int j) {
    if (!matrice || i < 0 || j < 0 || i >= matrice->nb_villes || j >= matrice->nb_villes) return -1.0;
    if (i == j) return 0.0;
    return (i < j) ? matrice->lignes[i][j-i-1] : matrice->lignes[j][i-j-1];
}

int definir_distance(DemiMatrice *matrice, int i, int j, double valeur) {
    if (!matrice || i < 0 || j < 0 || i >= matrice->nb_villes || j >= matrice->nb_villes) return -1;
    if (i == j) return 0;
    if (i < j) matrice->lignes[i][j-i-1] = valeur;
    else matrice->lignes[j][i-j-1] = valeur;
    return 0;
}

// Création à partir d'une tournée
DemiMatrice *demi_matrice_from_tour(tTournee tour, int nb_villes, DistanceFunc dist) {
    if (!tour || !dist || nb_villes <= 0) return NULL;

    DemiMatrice *matrice = creer_demi_matrice(nb_villes);
    if (!matrice) return NULL;

    for (int i = 0; i < nb_villes - 1; i++) {
        tInstance inst_i = get_instance_at(tour, i);
        for (int j = i + 1; j < nb_villes; j++) {
            tInstance inst_j = get_instance_at(tour, j);
            double d = dist(inst_i, inst_j);
            definir_distance(matrice, i, j, d);
        }
    }

    return matrice;
}

// Calcul de la longueur de la tournée via la demi-matrice
double tour_length_from_demi_matrice(tTournee tour, DemiMatrice *matrice) {
    if (!tour || !matrice) return 0.0;

    double total = 0.0;
    int i = 0;
    tInstance current, next;

    while ((current = get_instance_at(tour, i)) != NULL &&
           (next = get_instance_at(tour, i + 1)) != NULL) {
        total += obtenir_distance(matrice, i, i+1);
        i++;
    }

    // fermer le tour
    total += obtenir_distance(matrice, i, 0);
    return total;
}

