#include <stdlib.h>
#include <stdio.h>
#include "struct.h"

struct s_instance { 
    int id;
    double x;
    double y;
};

struct s_tournee { 
    tInstance *  chemin;
    int taille;
    int current;
};

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

double get_x(tInstance inst){
    return inst->x;
}
double get_y(tInstance inst){
    return inst->y;
}
int get_id(tInstance inst){
    return inst->id;
}

void delete_instance(tInstance * pinst){
    free(*pinst);
    *pinst = NULL;
}

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

int add_in_tournee(tTournee tour, tInstance inst){
    if(tour->current < tour->taille){
        tour->chemin[tour->current] = inst;
        tour->current++;
        return 0;
    }else{
        return -1;
    }
}

int get_taille_tournee(tTournee tour){
    return tour->current;
}

tInstance get_instance_at(tTournee tour,int nb){
    if (!tour) return NULL;
    if (nb >= tour->current) {
        return NULL;
    }

    return tour->chemin[nb];
}


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

int permute(tTournee tournee, int id1, int id2){
    if(id1 > tournee->current-1 || id2 > tournee->current-1){
        return -1;
    }
    tInstance tmp = tournee->chemin[id1];
    tournee->chemin[id1] = tournee->chemin[id2];
    tournee->chemin[id2] = tmp;
    return 0;
}
