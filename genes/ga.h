#ifndef GA_H
#define GA_H

#include "../tsp/struct.h"
#include "../distance/distance.h"

/* ----------------------------------------------------------
   Types génériques du GA
   ---------------------------------------------------------- */

typedef void* Individual;

typedef Individual (*GA_Copy)(Individual);
typedef void       (*GA_Delete)(Individual);
typedef void       (*GA_Mutate)(Individual, double);
typedef Individual (*GA_Crossover)(Individual, Individual, void *data);
typedef double     (*GA_Fitness)(Individual, void*);
typedef Individual (*GA_Selection)(Individual*, int, int, GA_Fitness, void*);
typedef Individual (*GA_CreateRandom)(int, void*);


/* ----------------------------------------------------------
   Données spécifiques au TSP
   ---------------------------------------------------------- */

typedef struct {
    DistanceFunc dist;
    tInstance *cities;
    int n;
} GA_Data;


/* ----------------------------------------------------------
   Paramètres du GA
   ---------------------------------------------------------- */

typedef struct {
    int population_size;
    int generations;
    int tournament_size;
    double mutation_rate;
    int individual_size;  
    void *data;
} GA_Parameters;


/* ----------------------------------------------------------
   Fonctions TSP (callbacks)
   ---------------------------------------------------------- */

Individual ga_copy_tour(Individual ind);
void       ga_delete_tour(Individual ind);
void       ga_mutate_tour(Individual ind, double mutation_rate);
Individual ga_ordered_crossover(Individual parent_a, Individual parent_b, void *data);
Individual ga_dpx_crossover(Individual parent_a, Individual parent_b, void *data);
double     ga_fitness_tour(Individual ind, void *data);
Individual ga_tournament_selection(Individual *population, int pop_size, int tournament_size,
                                   GA_Fitness fitness, void *data);
Individual ga_create_random_tour(int n, void *data);


/* ----------------------------------------------------------
   Algorithme génétique générique
   ---------------------------------------------------------- */

int ga_run(
    Individual *population,
    GA_Parameters *params,
    GA_Copy copy,
    GA_Delete del,
    GA_Mutate mutate,
    GA_Crossover crossover,
    GA_Fitness fitness,
    GA_Selection selection,
    GA_CreateRandom create_random,
    Individual *best_ind,
    double *best_score
);

#endif

