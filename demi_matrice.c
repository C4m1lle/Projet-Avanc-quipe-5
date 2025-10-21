#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "demi_matrice.h"


DemiMatrice *creer_demi_matrice(int nb_villes) {
    DemiMatrice *matrice = malloc(sizeof(DemiMatrice));
    if (matrice == NULL) {
        fprintf(stderr, "Erreur : allocation échouée pour la structure DemiMatrice.\n");
        return NULL;
    }

    matrice->nb_villes = nb_villes;

    matrice->lignes = malloc(nb_villes * sizeof(double *));
    if (matrice->lignes == NULL) {
        fprintf(stderr, "Erreur : allocation échouée pour les pointeurs de lignes.\n");
        free(matrice);
        return NULL;
    }

    for (int i = 0; i < nb_villes - 1; i++) {
        matrice->lignes[i] = malloc((nb_villes - i - 1) * sizeof(double));
        if (matrice->lignes[i] == NULL) {
            fprintf(stderr, "Erreur : allocation échouée pour la ligne %d.\n", i);
            for (int k = 0; k < i; k++)
                free(matrice->lignes[k]);
            free(matrice->lignes);
            free(matrice);
            return NULL;
        }

        for (int j = 0; j < nb_villes - i - 1; j++) {
            matrice->lignes[i][j] = 0.0;
        }
    }

    matrice->lignes[nb_villes - 1] = NULL; 
    return matrice;
}


void detruire_demi_matrice(DemiMatrice *matrice) {
    if (matrice == NULL) return;

    for (int i = 0; i < matrice->nb_villes - 1; i++) {
        free(matrice->lignes[i]);
    }
    free(matrice->lignes);
    free(matrice);
}


double obtenir_distance(const DemiMatrice *matrice, int i, int j) {
    if (matrice == NULL || i < 0 || j < 0 ||
        i >= matrice->nb_villes || j >= matrice->nb_villes) {
        fprintf(stderr, "Erreur : indices invalides dans obtenir_distance(%d, %d)\n", i, j);
        return -1.0;
    }

    if (i == j) return 0.0;

    if (i < j)
        return matrice->lignes[i][j - i - 1];
    else
        return matrice->lignes[j][i - j - 1];
}


int definir_distance(DemiMatrice *matrice, int i, int j, double valeur) {
    if (matrice == NULL || i < 0 || j < 0 ||
        i >= matrice->nb_villes || j >= matrice->nb_villes) {
        fprintf(stderr, "Erreur : indices invalides dans definir_distance(%d, %d)\n", i, j);
        return -1;
    }

    if (i == j) return 0;

    if (i < j)
        matrice->lignes[i][j - i - 1] = valeur;
    else
        matrice->lignes[j][i - j - 1] = valeur;

    return 0;
}


int remplir_matrice_depuis_instances(DemiMatrice *matrice, tInstance *villes, DistanceFunc fonction_distance) {
    if (matrice == NULL || villes == NULL || fonction_distance == NULL) {
        fprintf(stderr, "Erreur : pointeur NULL dans remplir_matrice_depuis_instances.\n");
        return -1;
    }

    for (int i = 0; i < matrice->nb_villes - 1; i++) {
        for (int j = i + 1; j < matrice->nb_villes; j++) {
            double d = fonction_distance(villes[i], villes[j]);
            definir_distance(matrice, i, j, d);
        }
    }
    return 0;
}
