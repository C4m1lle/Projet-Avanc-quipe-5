#include "struct.h"
#include "rw.h"
#include "nn.h"
#include "distance.h"
#include <stdlib.h>
#include <stdio.h>


int is_crossed(tInstance A,tInstance B,tInstance C,tInstance D){
    double a,b;
    double xc = get_x(C),xd = get_x(D),yc=get_y(C),yd=get_y(D);
    a = ((get_y(A)-get_y(B))/(get_x(A)-get_x(B)));
    b = get_y(A) - a*get_x(A);
    if(((a*xc+b > yc) && (a*xd+b < yd)) || ((a*xc+b < yc) && (a*xd+b > yd))){
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

    for(int i=0; i<taille_tournee-1; i++){ // optimisation de cette nouvelle tournee
        curr = get_instance_at(tmp,i);
        next = get_instance_at(tmp,i+1);
        for(int j=0;j<taille_tournee-1;j++){
            curr2 = get_instance_at(tmp,j);
            next2 = get_instance_at(tmp,j+1);
            if(j!=i && j!=i+1 && is_crossed(curr,next,curr2,next2)){
                reverse_segment(tmp,i+1,j);
            }
        }
    }
    for(int i = 0; i<taille_tournee; i++){ // reconstruction du tab apres optimisation
        tab_tournee[i] = get_id(get_instance_at(tmp,i));
    }
    dist = tour_length(tmp,dist_func);
    free(tmp);
    return dist;
}


