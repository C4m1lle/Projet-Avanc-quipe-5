#include "struct.h"
#include "rw.h"
#include "nn.h"
#include "distance.h"


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

void opt2(DistanceFunc dist_func,tTournee tournee){
    int taille_tournee = get_taille_tournee(tournee);
    tInstance curr,next, curr2, next2;
    for(int i=0; i<taille_tournee-1; i++){
        curr = get_instance_at(tournee,i);
        next = get_instance_at(tournee,i+1);
        for(int j=0;j<taille_tournee-1;j++){
            curr2 = get_instance_at(tournee,j);
            next2 = get_instance_at(tournee,j+1);
            if(j!=i && j!=i+1 && is_crossed(curr,next,curr2,next2)){
                if(dist_func(curr,curr2)<dist_func(curr,next2)){
                    permute(tournee,i,j);
                }else{
                    permute(tournee,i+1,j+1);
                }
            }
 
        }
    }
}


