/**
 * @file tspReader.h
 * @brief Déclarations pour la lecture des fichiers TSPLIB et gestion des problèmes TSP.
 */

#ifndef __TSPREADER_H__
#define __TSPREADER_H__	

/** 
 * @typedef tProbleme
 * @brief Pointeur vers la structure interne représentant un problème TSP.
 */
typedef struct s_probleme* tProbleme;

/**
 * @brief Charge un problème TSP depuis un fichier TSPLIB.
 * @param filepath Chemin vers le fichier .tsp
 * @return tProbleme Problème chargé, NULL en cas d'erreur
 */
tProbleme load_problem(const char * filepath);

/**
 * @brief Affiche les identifiants des villes d'un problème.
 * @param problem Problème TSP
 */
void print_nodes(tProbleme problem);

/**
 * @brief Récupère la tournée des villes du problème.
 * @param problem Problème TSP
 * @return tTournee Tournée des villes
 */
tTournee get_nodes(tProbleme problem);

/**
 * @brief Affiche les coordonnées des villes d'un problème.
 * @param problem Problème TSP
 */
void print_values(tProbleme problem);

/**
 * @brief Récupère le type de distance du problème.
 * @param problem Problème TSP
 * @return const char* Type de distance (EUC_2D, ATT, GEO)
 */
const char *get_edge_weight_type(tProbleme problem);

/**
 * @brief Supprime un problème et libère toutes les ressources associées.
 * @param pproblem Pointeur vers le problème
 */
void delete_problem(tProbleme *pproblem);

/**
 * @brief Récupère la taille (nombre de villes) du problème.
 * @param prob Problème TSP
 * @return int Nombre de villes
 */
int get_size_probleme(tProbleme prob);

#endif
