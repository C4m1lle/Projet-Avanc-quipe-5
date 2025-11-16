#ifndef __DEMI_MATRICE_H__
#define __DEMI_MATRICE_H__
#include "../distance/distance.h"


typedef struct sDemiMatrice * tDemiMatrice;

tDemiMatrice creer_demi_matrice(int nb_villes);
void detruire_demi_matrice(tDemiMatrice matrice);
double obtenir_distance(const tDemiMatrice matrice, int i, int j);
int definir_distance(tDemiMatrice matrice, int i, int j, double valeur);
tDemiMatrice demi_matrice_from_tour(tTournee tour, int nb_villes, DistanceFunc dist);
double tour_length_from_demi_matrice(tTournee tour, tDemiMatrice matrice);

#endif