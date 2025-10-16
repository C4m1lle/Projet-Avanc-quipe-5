#include <stdlib.h>
#include "struct.h"

struct s_instance { 
    int id;
    float x;
    float y;
};

struct s_tournee { 
    tInstance * chemin;
    int taille;
    int current;
};

tInstance create_instance(int idd, float xd, float yd){
    tInstance inst = malloc(sizeof(struct s_instance));
    inst->id = idd;
    inst->x = xd;
    inst->y = yd;
    return inst;
}
void delete_instance(tInstance * pinst){
    free(*pinst);
    *pinst = NULL;
}

tTournee create_tournee(int tailledef){
    tTournee tour = malloc(sizeof(struct s_tournee));
    tour->chemin = malloc(sizeof(tInstance)*tailledef);
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

void delete_tournee(tTournee * ptour){
    free((*ptour)->chemin);
    free(*ptour);
    *ptour = NULL;
}