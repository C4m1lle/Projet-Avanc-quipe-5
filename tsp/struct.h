/**
 * @file struct.h
 * @brief Déclarations des structures et fonctions pour les instances et tournées TSP.
 */

#ifndef __STRUCT_H__
#define __STRUCT_H__	

/**
 * @typedef tInstance
 * @brief Pointeur vers une structure représentant une ville du TSP.
 */
typedef struct s_instance* tInstance;

/**
 * @typedef tTournee
 * @brief Pointeur vers une structure représentant une tournée (séquence de villes).
 */
typedef struct s_tournee* tTournee;

/**
 * @brief Crée une instance (ville) avec un identifiant et des coordonnées.
 * @param idd Identifiant de la ville
 * @param xd Coordonnée x
 * @param yd Coordonnée y
 * @return tInstance Nouvelle instance créée
 */
tInstance create_instance(int idd, double xd, double yd);

/**
 * @brief Supprime une instance et libère la mémoire.
 * @param pinst Pointeur vers l'instance
 */
void delete_instance(tInstance * pinst);

/**
 * @brief Crée une tournée avec une taille initiale donnée.
 * @param tailledef Taille initiale de la tournée
 * @return tTournee Nouvelle tournée créée
 */
tTournee create_tournee(int tailledef);

/**
 * @brief Ajoute une instance à la tournée.
 * @param tour Tournée
 * @param inst Instance à ajouter
 * @return int 0 si succès, -1 si la tournée est pleine
 */
int add_in_tournee(tTournee tour, tInstance inst);

/**
 * @brief Supprime une tournée et toutes ses instances.
 * @param ptour Pointeur vers la tournée
 */
void delete_tournee(tTournee * ptour);

/**
 * @brief Récupère la coordonnée x d'une instance.
 * @param inst Instance
 * @return double Coordonnée x
 */
double get_x(tInstance inst);

/**
 * @brief Récupère la coordonnée y d'une instance.
 * @param inst Instance
 * @return double Coordonnée y
 */
double get_y(tInstance inst);

/**
 * @brief Récupère l'identifiant d'une instance.
 * @param inst Instance
 * @return int Identifiant
 */
int get_id(tInstance inst);

/**
 * @brief Récupère l'instance à un index donné dans une tournée.
 * @param tour Tournée
 * @param nb Index
 * @return tInstance Instance correspondante, NULL si index invalide
 */
tInstance get_instance_at(tTournee tour,int nb);

/**
 * @brief Récupère le nombre de villes ajoutées dans la tournée.
 * @param tour Tournée
 * @return int Nombre de villes
 */
int get_taille_tournee(tTournee tour);

/**
 * @brief Inverse un segment de la tournée entre deux indices.
 * @param tournee Tournée
 * @param id1 Index de début
 * @param id2 Index de fin
 * @return int 0 si succès, -1 si indices invalides
 */
int reverse_segment(tTournee tournee, int id1, int id2);

/**
 * @brief Supprime la tournée sans supprimer ses instances.
 * @param ptour Pointeur vers la tournée
 */
void delete_tournee_without_instances(tTournee *ptour);

/**
 * @brief Modifie l'instance à un index donné dans la tournée.
 * @param tour Tournée
 * @param index Index à modifier
 * @param inst Nouvelle instance
 */
void set_instance_at(tTournee tour, int index, tInstance inst);

/**
 * @brief Récupère le tableau des instances d'une tournée.
 * @param tour Tournée
 * @return tInstance* Tableau des instances
 */
tInstance * get_chemin_tournee(tTournee tour);

/**
 * @brief Récupère une instance par son identifiant.
 * @param tour Tournée
 * @param index Identifiant recherché
 * @return tInstance Instance correspondante, NULL si non trouvée
 */
tInstance get_instance_by_id(tTournee tour, int index);

#endif
