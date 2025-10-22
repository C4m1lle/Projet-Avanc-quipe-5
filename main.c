#include <stdlib.h>
#include <stdio.h>
#include "struct.h"
#include "distance.h"
#include "tspReader.h"

int main(int argc, char * argv[]){
    tInstance a = create_instance(1, 10.0, 10.0);
    tInstance b = create_instance(2, 20.0, 20.0);

    printf("Distance EUCL_2D : %.3f\n", dist_eucl2d(a, b));
    printf("Distance ATT     : %.3f\n", dist_att(a, b));

    delete_instance(&a);
    delete_instance(&b);

    // Exemple GEO 
    tInstance p = create_instance(1, 48.8566, 2.3522);
    tInstance r = create_instance(2, 41.9028, 12.4964);
    printf("Distance GEO     : %.3f km\n", dist_geo(p, r));

    delete_instance(&p);
    delete_instance(&r);

    tProbleme prob = load_problem("a280.tsp");
    print_nodes(prob);
    return 0;
}