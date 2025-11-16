#ifndef GA_H
#define GA_H


#include "../tsp/struct.h"
#include "../distance/distance.h"

// Génération initiale aléatoire
tTournee* random_population(int pop_size, tInstance* cities, int nb_cities);

// Sélection par tournoi
tTournee* tournament_selection(tTournee* population, int pop_size, int tournament_size, DistanceFunc dist);

// Croisement ordonné
tTournee ordered_crossover(tTournee parent_a, tTournee parent_b);

// Mutation par échange
void swap_mutation(tTournee tour, double mutation_rate);

// Tri simple de la population par longueur
void sort_population(tTournee* population, int pop_size, DistanceFunc dist);

// Algorithme génétique Python-like
int ga_tri_light(tTournee tour, DistanceFunc dist, int pop_size, int generations, double mutation_rate,
                   int** best_ids, double* best_dist);

#endif


