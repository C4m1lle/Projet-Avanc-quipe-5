#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "../tsp/struct.h"
#include "../distance/distance.h"
#include "demi_matrice.h"
#include "bruteforce.h"

int next_permutation(int ids[], int length){
	// Find non-increasing suffix
	if (length == 0)
		return 1;
	int i = length - 1;
	while (i > 0 && ids[i - 1] >= ids[i])
		i--;
	if (i == 0)
		return 1;
	
	// Find successor to pivot
	int j = length - 1;
	while (ids[j] <= ids[i - 1])
		j--;
	int temp = ids[i - 1];
	ids[i - 1] = ids[j];
	ids[j] = temp;
	
	// Reverse suffix
	j = length - 1;
	while (i < j) {
		temp = ids[i];
		ids[i] = ids[j];
		ids[j] = temp;
		i++;
		j--;
	}
	return 0;
}

void bruteforce(void ** tour, DistanceFuncGenerique dist,int * best, double * distmin,int lenght, int * has_to_stop){
    void * inst1;
    void *inst2;
    int taille = lenght;
    (*distmin) = DBL_MAX;
    double distcur;
    int * tab_id = malloc(sizeof(int)*taille);
    for(int i = 0;i<taille;i++){
        tab_id[i] = i+1;
    }
    while(next_permutation(tab_id,taille)==0 && (*has_to_stop)==0){
        distcur = 0;
        for(int i = 0;i<taille-1;i++){
            inst1 = tour[tab_id[i]-1];
            inst2 = tour[tab_id[i+1]-1];
            distcur+=dist(inst1,inst2);
        }
        distcur+=dist(tour[tab_id[0]-1],tour[tab_id[taille-1]-1]);
        
        if((*distmin)>distcur){
            (*distmin) = distcur;
            for(int i = 0;i<taille;i++){
                best[i] = tab_id[i];
            }
        }
    }
    free(tab_id);
}

void bruteforce_demi_matrice(void ** tour, DistanceFuncGenerique dist,int * best, double * distmin,int lenght, int * has_to_stop){
    tDemiMatrice matrice = demi_matrice_from_tour(tour,lenght,dist);


    int taille = lenght;
    (*distmin) = DBL_MAX;
    double distcur;
    int * tab_id = malloc(sizeof(int)*taille);
    for(int i = 0;i<taille;i++){
        tab_id[i] = i+1;
    }

    while(next_permutation(tab_id,taille)==0 && (*has_to_stop)==0){
        distcur = 0;
        for(int i = 0;i<taille-1;i++){
            distcur+=obtenir_distance(matrice,tab_id[i]-1,tab_id[i+1]-1);
        }
        distcur+=obtenir_distance(matrice,tab_id[0]-1,tab_id[taille-1]-1);
        if((*distmin)>distcur){
            (*distmin) = distcur;
            for(int i = 0;i<taille;i++){
                best[i] = tab_id[i];
            }
        }
    }
    free(tab_id);
    detruire_demi_matrice(matrice);
}
