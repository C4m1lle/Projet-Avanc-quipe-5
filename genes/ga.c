#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../tsp/struct.h"
#include "../distance/distance.h"
#include "ga.h"
#include <float.h>
#include "../heuristiques/opt2.h"
/* ==========================================================
   COPY — copie d’un individu (tour)
   ========================================================== */
Individual ga_copy_tour(Individual ind) {
    tTournee src = (tTournee) ind;
    int n = get_taille_tournee(src);

    tTournee dest = create_tournee(n);

    for (int i = 0; i < n; i++)
        add_in_tournee(dest, get_instance_at(src, i));

    return dest;
}

/* ==========================================================
   DELETE — suppression sans supprimer les villes
   ========================================================== */
void ga_delete_tour(Individual ind) {
    if (!ind) return;
    delete_tournee_without_instances((tTournee*)&ind);
}

/* ==========================================================
   MUTATION (swap mutation)
   ========================================================== */
void ga_mutate_tour(Individual ind, double mutation_rate) {
    tTournee tour = (tTournee) ind;
    int n = get_taille_tournee(tour);

    for (int i = 0; i < n; i++) {
        if ((double)rand() / RAND_MAX < mutation_rate) {
            int j = rand() % n;

            tInstance a = get_instance_at(tour, i);
            tInstance b = get_instance_at(tour, j);

            set_instance_at(tour, i, b);
            set_instance_at(tour, j, a);
        }
    }
}

/* ==========================================================
   FITNESS = longueur totale du tour
   ========================================================== */
double ga_fitness_tour(Individual ind, void *data) {
    GA_Data *g = (GA_Data*) data;
    return tour_length((tTournee) ind, g->dist);
}

/* ==========================================================
   SELECTION — tournament selection
   ========================================================== */
Individual ga_tournament_selection(Individual *population,int pop_size,int tournament_size,GA_Fitness fitness,void *data) {
    double best_score = -1.0;
    int best_idx = -1;

    for (int i = 0; i < tournament_size; i++) {
        int idx = rand() % pop_size;
        double score = fitness(population[idx], data);

        if (best_idx == -1 || score < best_score) {
            best_idx = idx;
            best_score = score;
        }
    }

    return ga_copy_tour(population[best_idx]);
}

/* ==========================================================
   CROSSOVER — Ordered Crossover (OX)
   ========================================================== */
Individual ga_ordered_crossover(Individual parent_a, Individual parent_b, void *data) {
    (GA_Data *)data;
    tTournee a = (tTournee)parent_a;
    tTournee b = (tTournee)parent_b;
    int n = get_taille_tournee(a);

    tTournee child = create_tournee(n);

    // Segment aléatoire
    int start = rand()%n;
    int end   = rand()%n;
    if(start > end){ int tmp=start; start=end; end=tmp; }

    // Tableau temporaire des positions
    tInstance* child_array = malloc(sizeof(tInstance) * n);
    for(int i=0;i<n;i++) child_array[i]=NULL;

    // 1) Copier segment de A
    for(int i=start;i<=end;i++)
        child_array[i] = get_instance_at(a,i);

    // 2) Compléter avec B
    int idx=0;
    for(int i=0;i<n;i++){
        tInstance inst = get_instance_at(b,i);

        // Vérifier si déjà dans la partie copiée
        int exists=0;
        for(int j=0;j<n;j++)
            if(child_array[j] && get_id(child_array[j]) == get_id(inst)){ exists=1; break; }

        if(!exists){
            while(child_array[idx]) idx++;
            child_array[idx] = inst;
        }
    }

    // Construire l’enfant final
    for(int i=0;i<n;i++)
        add_in_tournee(child, child_array[i]);

    free(child_array);
    return child;
}
Individual ga_dpx_crossover(Individual parent_a, Individual parent_b, void *data) {

    GA_Data  * tsp_data = (GA_Data *)data;
    tTournee a = (tTournee)parent_a;
    tTournee b = (tTournee)parent_b;
    int n = get_taille_tournee(a);

    tTournee child = create_tournee(n);
    tTournee childtmp = create_tournee(n);

    tInstance * child_array = malloc(sizeof(tInstance) * n);
    tInstance * intersec = malloc(sizeof(tInstance) * n);
    int missed = 0;

    //init des tableaux
    for(int i = 0; i<n; i++){
        intersec[i]=NULL;
        child_array[i]=NULL;
    }

    /* 1) Copier valeurs communes */
    for (int i = 0; i < n; i++){
        if (get_id(get_instance_at(a, i)) == get_id(get_instance_at(b, i))) {
            child_array[i] = get_instance_at(a, i);
        } else {
            intersec[missed++] = get_instance_at(a, i);
        }
    }

    /* 2) Remplir les trous */
    for (int i = 0; i < n; i++){

        if (child_array[i] == NULL) {

            /* Trouver le current précédent non vide */
            tInstance current = NULL;
            for (int k = i - 1; k >= 0 && current == NULL; k--){
                if (child_array[k] != NULL){
                    current = child_array[k];
                }
            }

            /* Si aucun précédent n’existe */
            if (current == NULL && missed > 0){
                current = intersec[0];  
            }

            /* Chercher le nearest */
            double best_d = DBL_MAX;
            int best_idx = -1;

            for (int k = 0; k < missed; k++){
                if (intersec[k] != NULL){
                    double d = tsp_data->dist(intersec[k], current);
                    if (d < best_d) {
                        best_d = d;
                        best_idx = k;
                    }
                }
            }

            /* Assigner et retirer */
            if (best_idx >= 0){
                child_array[i] = intersec[best_idx];
                intersec[best_idx] = NULL;
            }
        }
    }
    
    /* 3) Construire la tournée fille */
    int * tab_tournee = malloc(sizeof(int)*n);

    for (int i = 0; i < n; i++){
        add_in_tournee(childtmp, child_array[i]);
        tab_tournee[i] = get_id(child_array[i]);
    }

    //Optimisation de la tournée fille à l'aide de 2opt

    opt2((DistanceFunc)tsp_data->dist,childtmp,tab_tournee);

    for(int i = 0; i<n; i++){
        add_in_tournee(child,get_instance_by_id(childtmp,tab_tournee[i]));
        //printf("|%d|",tab_tournee[i]); //debug
        //printf("|%p|",get_chemin_tournee(child)[i]); //debug
        //fflush(stdout);
    }

    delete_tournee_without_instances(&childtmp);
    free(child_array);
    free(intersec);
    free(tab_tournee);

    return child;
}


/* ==========================================================
   RANDOM TOUR — création aléatoire
   ========================================================== */
Individual ga_create_random_tour(int n, void* data) {
    GA_Data *g = (GA_Data*) data;

    tTournee tour = create_tournee(n);

    int *idx = malloc(sizeof(int) * n);
    for (int i = 0; i < n; i++) idx[i] = i;

    //shuffle
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int t = idx[i]; idx[i] = idx[j]; idx[j] = t;
    }

    for (int i = 0; i < n; i++)
        add_in_tournee(tour, g->cities[idx[i]]);

    free(idx);
    return tour;
}

/* ==========================================================
   GA RUN — algo genetique generique
   ========================================================== */
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
) {
    if (!population || !params || params->population_size <= 0)
        return -1;

    int pop_size       = params->population_size;
    int generations    = params->generations;
    int tournament_sz  = params->tournament_size;
    int individual_sz  = params->individual_size;
    double mutation_r  = params->mutation_rate;
    void *data         = params->data;

    srand((unsigned int)time(NULL));

    /* Best = premier individu */
    Individual best = copy(population[0]);
    double best_len = fitness(best, data);

    /* BOUCLE GA */
    for (int g = 0; g < generations; g++) {

        /* 1) SELECTION */
        Individual *selected = malloc(sizeof(Individual) * pop_size);
        for (int i = 0; i < pop_size; i++)
            selected[i] = selection(population, pop_size, tournament_sz, fitness, data);

        /* 2) CROSSOVER */
        Individual *offspring = malloc(sizeof(Individual) * pop_size);
        for (int i = 0; i < pop_size; i += 2) {
            offspring[i]   = crossover(selected[i], selected[i+1], data);
            offspring[i+1] = crossover(selected[i+1], selected[i], data);
        }

        /* 3) MUTATION */
        for (int i = 0; i < pop_size; i++)
            mutate(offspring[i], mutation_r);

        /* 4) TRI*/
        for (int i = 0; i < pop_size - 1; i++)
            for (int j = i + 1; j < pop_size; j++)
                if (fitness(offspring[i], data) > fitness(offspring[j], data)) {
                    Individual tmp = offspring[i];
                    offspring[i] = offspring[j];
                    offspring[j] = tmp;
                }

        /* 5) REMPLACER LE PIRE PAR RANDOM TOUR */
        int worst = pop_size - 1;
        del(offspring[worst]);
        offspring[worst] = create_random(individual_sz, data);

        /* 6) GARDER LE MEILLEUR GLOBAL */
        if (fitness(offspring[0], data) < best_len) {
            del(best);
            best = copy(offspring[0]);
            best_len = fitness(best, data);
        }

        /* 7) remplacer par le meilleur individu*/
        del(offspring[worst]);
        offspring[worst] = copy(best);

        /* 8) Remplacement de la population */
        for (int i = 0; i < pop_size; i++) del(population[i]);
        free(population);
        population = offspring;

        /* Libération de selected */
        for (int i = 0; i < pop_size; i++) del(selected[i]);
        free(selected);
    }

    /* Retour des résultats */
    *best_ind = best;
    *best_score = best_len;

    /* Nettoyage */
    for (int i = 0; i < pop_size; i++) del(population[i]);
    free(population);

    return 0;
}

