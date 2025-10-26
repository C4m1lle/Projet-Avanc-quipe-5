#ifndef __BRUTEFORCE_H__
#define __BRUTEFORCE_H__	


void bruteforce(tTournee tour, DistanceFunc dist, int * best, double * distmin,int * has_to_stop);
void bruteforce_demi_matrice(tTournee tour,tDemiMatrice matrice,int *best,double *distmin,int *has_to_stop);

#endif