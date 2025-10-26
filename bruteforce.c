#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "struct.h"
#include "distance.h"
#include "demi_matrice.h"

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

void bruteforce(tTournee tour, DistanceFunc dist,int * best, double * distmin,int * has_to_stop){
    tInstance inst1,inst2;
    int taille = get_taille_tournee(tour);
    (*distmin) = DBL_MAX;
    double distcur;
    int * tab_id = malloc(sizeof(int)*taille);
    for(int i = 0;i<taille;i++){
        inst1 = get_instance_at(tour,i);
        tab_id[i] = get_id(inst1);
    }
    while(next_permutation(tab_id,taille)==0 && (*has_to_stop)==0){
        distcur = 0;
        for(int i = 0;i<taille-1;i++){
            inst1 = get_instance_at(tour,(tab_id[i])-1);
            inst2 = get_instance_at(tour,(tab_id[i+1])-1);
            distcur+=dist(inst1,inst2);
        }
        
        if((*distmin)>distcur){
            (*distmin) = distcur;
            for(int i = 0;i<taille;i++){
                best[i] = tab_id[i];
            }
        }
    }
    free(tab_id);
}

void bruteforce_demi_matrice(tTournee tour, DistanceFunc dist,int * best, double * distmin,int * has_to_stop){
    tDemiMatrice matrice = demi_matrice_from_tour(tour,get_taille_tournee(tour),dist);
    tInstance inst = NULL;

    int taille = get_taille_tournee(tour);
    (*distmin) = DBL_MAX;
    double distcur;
    int * tab_id = malloc(sizeof(int)*taille);
    for(int i = 0;i<taille;i++){
        inst = get_instance_at(tour,i);
        tab_id[i] = get_id(inst);
    }

    while(next_permutation(tab_id,taille)==0 && (*has_to_stop)==0){
        distcur = 0;
        for(int i = 0;i<taille-1;i++){
            distcur+=obtenir_distance(matrice,tab_id[i]-1,tab_id[i+1]-1);
        }
        
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
