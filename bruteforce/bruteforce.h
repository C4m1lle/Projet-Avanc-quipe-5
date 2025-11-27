#ifndef __BRUTEFORCE_H__
#define __BRUTEFORCE_H__	

#include "../tsp/types.h"

void bruteforce(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght, int * has_to_stop);
void bruteforce_demi_matrice(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght, int * has_to_stop);
//has_to_stop est une variable permettant l'arrêt du bruteforce à tout moment par l'implémentation d'un handler
#endif