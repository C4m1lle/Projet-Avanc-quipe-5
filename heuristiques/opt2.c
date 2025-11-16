#include "../tsp/struct.h"
#include "rw.h"
#include "nn.h"
#include "../distance/distance.h"
#include <stdlib.h>
#include <stdio.h>


int is_crossed(tInstance A, tInstance B, tInstance C, tInstance D){
    double ax = get_x(A), ay = get_y(A);
    double bx = get_x(B), by = get_y(B);
    double cx = get_x(C), cy = get_y(C);
    double dx = get_x(D), dy = get_y(D);

    double d1, d2, d3, d4;

    // produits vectoriels
    d1 = (bx - ax)*(cy - ay) - (by - ay)*(cx - ax);
    d2 = (bx - ax)*(dy - ay) - (by - ay)*(dx - ax);
    d3 = (dx - cx)*(ay - cy) - (dy - cy)*(ax - cx);
    d4 = (dx - cx)*(by - cy) - (dy - cy)*(bx - cx);

    // segments croisés si les signes sont opposés
    if((d1 * d2 < 0) && (d3 * d4 < 0)){
        return 1;
    }
    return 0;
}




double opt2(DistanceFunc dist_func,tTournee tournee, int * tab_tournee){
    int taille_tournee = get_taille_tournee(tournee);
    int j;
    double dist;
    tTournee tmp = create_tournee(taille_tournee);
    for(int i = 0; i<taille_tournee;i++){ // creation d'une tournee à partir du tableau  besttournee calculé auparavant par nn ou rw
        j = 0;
        while(j<taille_tournee && get_id(get_instance_at(tournee,j))!=tab_tournee[i]){
            j++;
        }
        add_in_tournee(tmp,get_instance_at(tournee,j));
    }


    tInstance curr,next, curr2, next2;

int improved = 1;
    while(improved){
            improved = 0;
            for(int i=0; i<taille_tournee-3; i++){ // optimisation de cette nouvelle tournee
                curr = get_instance_at(tmp,i);
                next = get_instance_at(tmp,i+1);
                for(int j=i+2;j<taille_tournee-1;j++){
                    curr2 = get_instance_at(tmp,j);
                    next2 = get_instance_at(tmp,j+1);
                    if(is_crossed(curr,next,curr2,next2)){
                        reverse_segment(tmp,i+1,j);
                        improved = 1;
                    }
                }
            }
        }
    for(int i = 0; i<taille_tournee; i++){ // reconstruction du tab apres optimisation
        tab_tournee[i] = get_id(get_instance_at(tmp,i));
    }
    dist = tour_length(tmp,dist_func);
    delete_tournee_without_instances(&tmp);
    return dist;
}


