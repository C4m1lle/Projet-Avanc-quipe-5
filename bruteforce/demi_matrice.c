/**
 * @file demi_matrice.c
 * @brief Gestion d'une demi-matrice pour le calcul des distances entre villes.
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "demi_matrice.h"
#include "../tsp/struct.h"
#include "../distance/distance.h"

/**
 * @struct sDemiMatrice
 * @brief Représentation d'une demi-matrice symétrique pour stocker les distances entre villes.
 *
 * @var sDemiMatrice::nb_villes Nombre total de villes.
 * @var sDemiMatrice::lignes Tableau de tableaux de distances, ne stocke que la moitié supérieure.
 */
struct sDemiMatrice{
    int nb_villes;
    double **lignes;
};

/**
 * @brief Crée une demi-matrice vide pour un nombre donné de villes.
 *
 * @param nb_villes Nombre de villes.
 * @return tDemiMatrice Nouvelle demi-matrice ou NULL si l'allocation échoue.
 */
tDemiMatrice creer_demi_matrice(int nb_villes) {
    tDemiMatrice matrice = malloc(sizeof(struct sDemiMatrice));
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

/**
 * @brief Libère la mémoire occupée par une demi-matrice.
 *
 * @param matrice Demi-matrice à détruire.
 */
void detruire_demi_matrice(tDemiMatrice matrice) {
    if (!matrice) return;
    for (int i = 0; i < matrice->nb_villes - 1; i++) free(matrice->lignes[i]);
    free(matrice->lignes);
    free(matrice);
}

/**
 * @brief Obtient la distance entre deux villes dans une demi-matrice.
 *
 * @param matrice Demi-matrice contenant les distances.
 * @param i Index de la première ville.
 * @param j Index de la deuxième ville.
 * @return Distance entre les deux villes, ou -1.0 si indices invalides, 0.0 si i == j.
 */
double obtenir_distance(const tDemiMatrice matrice, int i, int j) {
    if (!matrice || i < 0 || j < 0 || i >= matrice->nb_villes || j >= matrice->nb_villes) return -1.0;
    if (i == j) return 0.0;
    return (i < j) ? matrice->lignes[i][j-i-1] : matrice->lignes[j][i-j-1];
}

/**
 * @brief Définit la distance entre deux villes dans une demi-matrice.
 *
 * @param matrice Demi-matrice contenant les distances.
 * @param i Index de la première ville.
 * @param j Index de la deuxième ville.
 * @param valeur Valeur de la distance à définir.
 * @return 0 si succès, -1 si indices invalides, 0 si i == j.
 */
int definir_distance(tDemiMatrice matrice, int i, int j, double valeur) {
    if (!matrice || i < 0 || j < 0 || i >= matrice->nb_villes || j >= matrice->nb_villes) return -1;
    if (i == j) return 0;
    if (i < j) matrice->lignes[i][j-i-1] = valeur;
    else matrice->lignes[j][i-j-1] = valeur;
    return 0;
}

/**
 * @brief Crée une demi-matrice à partir d'une tournée et d'une fonction de distance.
 *
 * @param tour Tableau de pointeurs vers les villes.
 * @param nb_villes Nombre de villes.
 * @param dist Fonction générique de calcul de distance.
 * @return Demi-matrice remplie avec les distances, ou NULL si erreur.
 */
tDemiMatrice demi_matrice_from_tour(void** tour, int nb_villes, DistanceFuncGenerique dist) {
    if (!tour || !dist || nb_villes <= 0) return NULL;

    tDemiMatrice matrice = creer_demi_matrice(nb_villes);
    if (!matrice) return NULL;

    for (int i = 0; i < nb_villes - 1; i++) {
        void * inst_i = tour[i];
        for (int j = i + 1; j < nb_villes; j++) {
            void * inst_j = tour[j];
            double d = dist(inst_i, inst_j);
            definir_distance(matrice, i, j, d);
        }
    }

    return matrice;
}

/**
 * @brief Calcule la longueur totale d'une tournée en utilisant une demi-matrice.
 *
 * @param tour Tournée à mesurer.
 * @param matrice Demi-matrice contenant les distances entre villes.
 * @return Longueur totale de la tournée.
 */
double tour_length_from_demi_matrice(tTournee tour, tDemiMatrice matrice) {
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
