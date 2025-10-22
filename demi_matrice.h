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

int remplir_matrice_depuis_instances(DemiMatrice *matrice, tInstance *villes, DistanceFunc fonction_distance);

#endif

