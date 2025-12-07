/**
 * @file demi_matrice.h
 * @brief Déclarations pour la gestion des demi-matrices de distances entre villes.
 */

#ifndef __DEMI_MATRICE_H__
#define __DEMI_MATRICE_H__

#include "../distance/distance.h"
#include "../tsp/types.h"

/**
 * @typedef tDemiMatrice
 * @brief Pointeur vers une structure représentant une demi-matrice symétrique de distances.
 */
typedef struct sDemiMatrice * tDemiMatrice;

/**
 * @brief Crée une demi-matrice vide pour un nombre donné de villes.
 *
 * @param nb_villes Nombre de villes.
 * @return tDemiMatrice Nouvelle demi-matrice ou NULL si l'allocation échoue.
 */
tDemiMatrice creer_demi_matrice(int nb_villes);

/**
 * @brief Libère la mémoire occupée par une demi-matrice.
 *
 * @param matrice Demi-matrice à détruire.
 */
void detruire_demi_matrice(tDemiMatrice matrice);

/**
 * @brief Obtient la distance entre deux villes dans une demi-matrice.
 *
 * @param matrice Demi-matrice contenant les distances.
 * @param i Index de la première ville.
 * @param j Index de la deuxième ville.
 * @return Distance entre les deux villes, ou -1.0 si indices invalides, 0.0 si i == j.
 */
double obtenir_distance(const tDemiMatrice matrice, int i, int j);

/**
 * @brief Définit la distance entre deux villes dans une demi-matrice.
 *
 * @param matrice Demi-matrice contenant les distances.
 * @param i Index de la première ville.
 * @param j Index de la deuxième ville.
 * @param valeur Valeur de la distance à définir.
 * @return 0 si succès, -1 si indices invalides, 0 si i == j.
 */
int definir_distance(tDemiMatrice matrice, int i, int j, double valeur);

/**
 * @brief Crée une demi-matrice à partir d'une tournée et d'une fonction de distance.
 *
 * @param tour Tableau de pointeurs vers les villes.
 * @param nb_villes Nombre de villes.
 * @param dist Fonction générique de calcul de distance.
 * @return Demi-matrice remplie avec les distances, ou NULL si erreur.
 */
tDemiMatrice demi_matrice_from_tour(void ** tour, int nb_villes, DistanceFuncGenerique dist);

/**
 * @brief Calcule la longueur totale d'une tournée en utilisant une demi-matrice.
 *
 * @param tour Tournée à mesurer.
 * @param matrice Demi-matrice contenant les distances entre villes.
 * @return Longueur totale de la tournée.
 */
double tour_length_from_demi_matrice(tTournee tour, tDemiMatrice matrice);

#endif
