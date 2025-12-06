/**
 * @file struct.c
 * @brief Implémentation des structures TSP : instances et tournées.
 */

#include <stdlib.h>
#include <stdio.h>
#include "struct.h"

/**
 * @struct s_instance
 * @brief Représente une ville du TSP avec un identifiant et ses coordonnées.
 */
struct s_instance { 
    int id;      /**< Identifiant unique de la ville */
    double x;    /**< Coordonnée x */
    double y;    /**< Coordonnée y */
};

/**
 * @struct s_tournee
 * @brief Représente une tournée (séquence de villes) pour le TSP.
 */
struct s_tournee { 
    tInstance *chemin; /**< Tableau d'instances représentant la tournée */
    int taille;        /**< Taille allouée du tableau */
    int current;       /**< Nombre actuel de villes ajoutées */
};

/**
 * @brief Crée une instance (ville) avec un ID et des coordonnées.
 * 
 * @param idd Identifiant de la ville
 * @param xd Coordonnée x
 * @param yd Coordonnée y
 * @return tInstance Nouvelle instance créée, NULL si échec d'allocation
 */
tInstance create_instance(int idd, double xd, double yd){
    tInstance inst = malloc(sizeof(struct s_instance));
    if(inst == NULL){
        fprintf(stderr, "Erreur : allocation échouée pour la structure \"Instance\"(%d,%lf,%lf).\n",idd,xd,yd);
        return NULL;
    }
    inst->id = idd;
    inst->x = xd;
    inst->y = yd;
    return inst;
}

/**
 * @brief Récupère la coordonnée x d'une instance.
 * @param inst Instance
 * @return double Coordonnée x
 */
double get_x(tInstance inst){
    return inst->x;
}

/**
 * @brief Récupère la coordonnée y d'une instance.
 * @param inst Instance
 * @return double Coordonnée y
 */
double get_y(tInstance inst){
    return inst->y;
}

/**
 * @brief Récupère l'identifiant d'une instance.
 * @param inst Instance
 * @return int Identifiant
 */
int get_id(tInstance inst){
    return inst->id;
}

/**
 * @brief Supprime une instance et libère la mémoire associée.
 * @param pinst Pointeur vers l'instance
 */
void delete_instance(tInstance * pinst){
    free(*pinst);
    *pinst = NULL;
}

/**
 * @brief Crée une tournée avec une taille initiale donnée.
 * @param tailledef Taille allouée pour la tournée
 * @return tTournee Nouvelle tournée, NULL si échec d'allocation
 */
tTournee create_tournee(int tailledef){
    tTournee tour = malloc(sizeof(struct s_tournee));
    if(tour == NULL){
        fprintf(stderr, "Erreur : allocation échouée pour la structure \"Tournee\"\n");
        return NULL;
    }
    tour->chemin = malloc(sizeof(tInstance)*tailledef);
    if(tour->chemin == NULL){
        fprintf(stderr, "Erreur : allocation échouée pour la structure \"Tournee\".\n");
        free(tour);
        return NULL;
    }
    tour->taille = tailledef;
    tour->current = 0;
    return tour;
}

/**
 * @brief Ajoute une instance dans une tournée.
 * @param tour Tournée
 * @param inst Instance à ajouter
 * @return int 0 si succès, -1 si la tournée est pleine
 */
int add_in_tournee(tTournee tour, tInstance inst){
    if(tour->current < tour->taille){
        tour->chemin[tour->current] = inst;
        tour->current++;
        return 0;
    }else{
        return -1;
    }
}

/**
 * @brief Récupère le nombre de villes dans la tournée.
 * @param tour Tournée
 * @return int Nombre de villes actuellement ajoutées
 */
int get_taille_tournee(tTournee tour){
    return tour->current;
}

/**
 * @brief Récupère l'instance à une position donnée dans la tournée.
 * @param tour Tournée
 * @param nb Index
 * @return tInstance Instance à l'index nb, NULL si index invalide
 */
tInstance get_instance_at(tTournee tour,int nb){
    if (!tour) return NULL;
    if (nb >= tour->current) {
        return NULL;
    }
    return tour->chemin[nb];
}

/**
 * @brief Supprime une tournée et toutes ses instances.
 * @param ptour Pointeur vers la tournée
 */
void delete_tournee(tTournee * ptour){
   if (!ptour || !*ptour) return;

    tTournee t = *ptour;

    for (int i = 0; i < t->current; i++) {
        delete_instance(&(t->chemin[i]));
    }

    free(t->chemin);
    free(t);
    *ptour = NULL;
}

/**
 * @brief Inverse un segment d'une tournée entre deux indices.
 * @param tournee Tournée
 * @param id1 Index de départ
 * @param id2 Index de fin
 * @return int 0 si succès, -1 si indices invalides
 */
int reverse_segment(tTournee tournee, int id1, int id2){
    if(id1 > tournee->current-1 || id2 > tournee->current-1){
        return -1;
    }
    tInstance tmp;
    while (id1 < id2) {
            tmp = tournee->chemin[id1];
            tournee->chemin[id1] = tournee->chemin[id2];
            tournee->chemin[id2] = tmp;
            id1++;
            id2--;
        }
    return 0;
}

/**
 * @brief Supprime une tournée sans supprimer ses instances.
 * @param ptour Pointeur vers la tournée
 */
void delete_tournee_without_instances(tTournee *ptour) {
    if (!ptour || !*ptour) return;

    tTournee t = *ptour;

    if (t->chemin) {
        free(t->chemin);
    }

    free(t);
    *ptour = NULL;
}

/**
 * @brief Modifie l'instance à un index donné dans la tournée.
 * @param tour Tournée
 * @param index Index de modification
 * @param inst Nouvelle instance
 */
void set_instance_at(tTournee tour, int index, tInstance inst){
    if (!tour || index<0 || index>=tour->current) return;
    tour->chemin[index] = inst;
}

/**
 * @brief Récupère le tableau des instances d'une tournée.
 * @param tour Tournée
 * @return tInstance* Tableau des instances, NULL si tour vide
 */
tInstance * get_chemin_tournee(tTournee tour){
    if(tour){
        return tour->chemin;
    }
    return NULL;
}

/**
 * @brief Récupère une instance par son identifiant.
 * @param tour Tournée
 * @param index Identifiant recherché
 * @return tInstance Instance correspondante, NULL si non trouvée
 */
tInstance get_instance_by_id(tTournee tour, int index){
    if(!tour){
        return NULL;
    }
    int i = 0,n = tour->current;
    if(index <= n && index > 0){
        while(i<n && get_id(tour->chemin[i])!=index){
            i++;
        }
        return tour->chemin[i];
    }
    return NULL;
}
