#ifndef __DEMI_MATRICE_H__
#define __DEMI_MATRICE_H__

#include "struct.h"
#include "distance.h"  

typedef struct {
    int nb_villes;
    double **lignes;
} DemiMatrice;

DemiMatrice *creer_demi_matrice(int nb_villes);
void detruire_demi_matrice(DemiMatrice *matrice);
double obtenir_distance(const DemiMatrice *matrice, int i, int j);
int definir_distance(DemiMatrice *matrice, int i, int j, double valeur);
DemiMatrice *demi_matrice_from_tour(tTournee tour, int nb_villes, DistanceFunc dist);
double tour_length_from_demi_matrice(tTournee tour, DemiMatrice *matrice);

#endif