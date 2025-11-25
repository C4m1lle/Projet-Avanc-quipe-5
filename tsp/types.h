#ifndef __TSP_TYPES_H__
#define __TSP_TYPES_H__

/* Structures d'appel commune à toutes les méthodes de calcul*/
typedef double (*DistanceFuncGenerique)(void *, void *);
typedef void (*solving_method)(void ** tournee, DistanceFuncGenerique dist_method, int * best_path, double * dist_best_path, int lenght_tournee);


#endif