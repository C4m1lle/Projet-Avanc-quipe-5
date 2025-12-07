/**
 * @file ga.h
 * @brief Déclarations et types pour un algorithme génétique appliqué au TSP.
 */

#ifndef GA_H
#define GA_H

#include "../tsp/struct.h"
#include "../distance/distance.h"

/* ----------------------------------------------------------
   Types génériques du GA
   ---------------------------------------------------------- */

/**
 * @typedef Individual
 * @brief Représente un individu dans le GA (pointeur générique vers une tournée).
 */
typedef void* Individual;

/**
 * @brief Fonction de copie d'un individu.
 */
typedef Individual (*GA_Copy)(Individual);

/**
 * @brief Fonction de suppression d'un individu.
 */
typedef void (*GA_Delete)(Individual);

/**
 * @brief Fonction de mutation d'un individu.
 */
typedef void (*GA_Mutate)(Individual, double);

/**
 * @brief Fonction de crossover entre deux individus.
 */
typedef Individual (*GA_Crossover)(Individual, Individual, void *data);

/**
 * @brief Fonction de fitness pour évaluer un individu.
 */
typedef double (*GA_Fitness)(Individual, void*);

/**
 * @brief Fonction de sélection d'un individu dans la population.
 */
typedef Individual (*GA_Selection)(Individual*, int, int, GA_Fitness, void*);

/**
 * @brief Fonction de création d'un individu aléatoire.
 */
typedef Individual (*GA_CreateRandom)(int, void*);

/* ----------------------------------------------------------
   Données spécifiques au TSP
   ---------------------------------------------------------- */

/**
 * @struct GA_Data
 * @brief Données supplémentaires pour le GA appliqué au TSP.
 */
typedef struct {
    DistanceFuncGenerique dist; /**< Fonction de distance */
    tInstance *cities;          /**< Tableau des instances */
    int n;                      /**< Nombre de villes */
} GA_Data;

/* ----------------------------------------------------------
   Paramètres du GA
   ---------------------------------------------------------- */

/**
 * @struct GA_Parameters
 * @brief Paramètres de configuration de l'algorithme génétique.
 */
typedef struct {
    int population_size;  /**< Taille de la population */
    int generations;      /**< Nombre de générations */
    int tournament_size;  /**< Taille du tournoi pour la sélection */
    double mutation_rate; /**< Probabilité de mutation par position */
    int individual_size;  /**< Taille d'un individu (nombre de villes) */
    void *data;           /**< Données supplémentaires (GA_Data) */
} GA_Parameters;

/* ----------------------------------------------------------
   Fonctions TSP (callbacks)
   ---------------------------------------------------------- */

/**
 * @brief Copie un individu (tournée).
 */
Individual ga_copy_tour(Individual ind);

/**
 * @brief Supprime un individu sans libérer les instances.
 */
void ga_delete_tour(Individual ind);

/**
 * @brief Applique une mutation (swap) sur un individu.
 */
void ga_mutate_tour(Individual ind, double mutation_rate);

/**
 * @brief Applique le crossover Ordered Crossover (OX) entre deux parents.
 */
Individual ga_ordered_crossover(Individual parent_a, Individual parent_b, void *data);

/**
 * @brief Applique le crossover DPX entre deux parents et optimise avec 2-opt.
 */
Individual ga_dpx_crossover(Individual parent_a, Individual parent_b, void *data);

/**
 * @brief Calcule la fitness d'un individu (longueur totale du tour).
 */
double ga_fitness_tour(Individual ind, void *data);

/**
 * @brief Sélectionne un individu via le tournoi.
 */
Individual ga_tournament_selection(Individual *population, int pop_size, int tournament_size,
                                   GA_Fitness fitness, void *data);

/**
 * @brief Crée une tournée aléatoire.
 */
Individual ga_create_random_tour(int n, void *data);

/* ----------------------------------------------------------
   Algorithme génétique générique
   ---------------------------------------------------------- */

/**
 * @brief Exécute l'algorithme génétique complet sur une population.
 *
 * @param population Population initiale.
 * @param params Paramètres du GA.
 * @param copy Fonction de copie d'un individu.
 * @param del Fonction de suppression d'un individu.
 * @param mutate Fonction de mutation d'un individu.
 * @param crossover Fonction de crossover entre deux individus.
 * @param fitness Fonction de fitness.
 * @param selection Fonction de sélection.
 * @param create_random Fonction de création d'individus aléatoires.
 * @param best_ind Pointeur pour stocker le meilleur individu trouvé.
 * @param best_score Pointeur pour stocker la meilleure fitness trouvée.
 * @return 0 si succès, -1 si erreur.
 */
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
