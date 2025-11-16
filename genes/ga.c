#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ga.h"
#include "../tsp/struct.h"
#include "../distance/distance.h"


// Génération initiale
tTournee* random_population(int pop_size, tInstance* cities, int nb_cities) {
    tTournee* population = malloc(sizeof(tTournee) * pop_size);
    for (int k = 0; k < pop_size; k++) {
        population[k] = create_tournee(nb_cities);
        int* indices = malloc(sizeof(int) * nb_cities);
        for (int i = 0; i < nb_cities; i++) indices[i] = i;

        //random shuffle
        for (int i = nb_cities - 1; i > 0; i--) {
            int j = rand() % (i+1);
            int tmp = indices[i]; indices[i] = indices[j]; indices[j] = tmp;
        }

        for (int i = 0; i < nb_cities; i++)
            add_in_tournee(population[k], cities[indices[i]]);
        free(indices);
    }
    return population;
}

// Sélection par tournoi
tTournee* tournament_selection(tTournee* population, int pop_size, int tournament_size, DistanceFunc dist) {
    tTournee* selected = malloc(sizeof(tTournee) * pop_size);
    for (int i = 0; i < pop_size; i++) {
        double best_score = -1.0;
        int best_idx = -1;
        for (int j = 0; j < tournament_size; j++) {
            int idx = rand() % pop_size;
            double score = tour_length(population[idx], dist);
            if (best_idx == -1 || score < best_score) {
                best_score = score;
                best_idx = idx;
            }
        }
        int n_cities = get_taille_tournee(population[best_idx]);
        selected[i] = create_tournee(n_cities);
        for (int k = 0; k < n_cities; k++)
            add_in_tournee(selected[i], get_instance_at(population[best_idx], k));
    }
    return selected;
}

// Croisement ordonné
tTournee ordered_crossover(tTournee parent_a, tTournee parent_b) {
    int size = get_taille_tournee(parent_a);
    tTournee child = create_tournee(size);

    int start = rand() % size;
    int end = rand() % size;
    if (start > end) { int tmp=start; start=end; end=tmp; }

    for (int i = start; i <= end; i++)
        add_in_tournee(child, get_instance_at(parent_a,i));

    for (int i = 0; i < size; i++) {
        tInstance inst = get_instance_at(parent_b,i);
        int exists = 0;
        for (int j = 0; j < get_taille_tournee(child); j++)
            if (get_id(get_instance_at(child,j)) == get_id(inst)) { exists = 1; break; }
        if (!exists) add_in_tournee(child, inst);
    }
    return child;
}

// Mutation par échange
void swap_mutation(tTournee tour, double mutation_rate) {
    int n = get_taille_tournee(tour);
    for (int i = 0; i < n; i++) {
        if ((double)rand()/RAND_MAX < mutation_rate) {
            int j = rand() % n;
            tInstance tmp_i = get_instance_at(tour,i);
            tInstance tmp_j = get_instance_at(tour,j);
            set_instance_at(tour,i,tmp_j);
            set_instance_at(tour,j,tmp_i);
        }
    }
}

// Tri simple par longueur
void sort_population(tTournee* population, int pop_size, DistanceFunc dist) {
    for (int i = 0; i < pop_size-1; i++) {
        for (int j = i+1; j < pop_size; j++) {
            if (tour_length(population[i], dist) > tour_length(population[j], dist)) {
                tTournee tmp = population[i];
                population[i] = population[j];
                population[j] = tmp;
            }
        }
    }
}

// Algorithme génétique
int ga_tri_light(tTournee tour, DistanceFunc dist, int pop_size, int generations, double mutation_rate,
                   int** best_ids, double* best_dist) {

    int nb_cities = get_taille_tournee(tour);
    int tournament_size = (int)(pop_size * 0.7);

    srand((unsigned int)time(NULL));

    tInstance* cities = malloc(sizeof(tInstance) * nb_cities);
    for (int i = 0; i < nb_cities; i++) cities[i] = get_instance_at(tour,i);

    tTournee* population = random_population(pop_size, cities, nb_cities);

    tTournee best_ind = create_tournee(nb_cities);
    for (int i=0; i<nb_cities; i++) add_in_tournee(best_ind, cities[i]);
    double best_length = tour_length(best_ind, dist);

    for (int g=0; g<generations; g++) {
        tTournee* selected = tournament_selection(population, pop_size, tournament_size, dist);

        tTournee* offspring = malloc(sizeof(tTournee) * pop_size);
        for (int i=0; i<pop_size; i+=2) {
            offspring[i]   = ordered_crossover(selected[i], selected[i+1]);
            offspring[i+1] = ordered_crossover(selected[i+1], selected[i]);
        }

        for (int i=0; i<pop_size; i++)
            swap_mutation(offspring[i], mutation_rate);

        sort_population(offspring, pop_size, dist);

        // Remplacement du pire par un nouveau
        int worst_idx = pop_size-1;
        delete_tournee_without_instances(&offspring[worst_idx]);
        offspring[worst_idx] = create_tournee(nb_cities);
        for (int i=0; i<nb_cities; i++) add_in_tournee(offspring[worst_idx], cities[i]);
        for (int i = nb_cities-1; i>0; i--) {
            int j = rand() % (i+1);
            tInstance tmp = get_instance_at(offspring[worst_idx], i);
            set_instance_at(offspring[worst_idx], i, get_instance_at(offspring[worst_idx], j));
            set_instance_at(offspring[worst_idx], j, tmp);
        }

        // Mise à jour meilleur génération
        if (tour_length(offspring[0], dist) < best_length) {
            delete_tournee_without_instances(&best_ind);
            best_ind = create_tournee(nb_cities);
            for (int i=0; i<nb_cities; i++)
                add_in_tournee(best_ind, get_instance_at(offspring[0], i));
            best_length = tour_length(best_ind, dist);
        }

        // Réinsertion du meilleur
        delete_tournee_without_instances(&offspring[worst_idx]);
        offspring[worst_idx] = create_tournee(nb_cities);
        for (int i=0; i<nb_cities; i++)
            add_in_tournee(offspring[worst_idx], get_instance_at(best_ind, i));

        // Nettoyage
        for (int i=0; i<pop_size; i++) delete_tournee_without_instances(&population[i]);
        free(population);
        population = offspring;

        for (int i=0; i<pop_size; i++) delete_tournee_without_instances(&selected[i]);
        free(selected);
    }

    *best_ids = malloc(sizeof(int) * nb_cities);
    for (int i=0; i<nb_cities; i++) (*best_ids)[i] = get_id(get_instance_at(best_ind, i));
    *best_dist = best_length;

    delete_tournee_without_instances(&best_ind);
    for (int i=0; i<pop_size; i++) delete_tournee_without_instances(&population[i]);
    free(population);
    free(cities);

    return 0;
}










