#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "distance.h"
#include "../tsp/struct.h"
#define PI 3.141592
#define RRR 6378.388
static int nint(double x) {
    return (int)(x + 0.5);
}
// --------------------------------------------
// Distance EUCLIDIENNE (EUCL_2D)
// --------------------------------------------
double dist_eucl2d(tInstance a, tInstance b) {
    double xd = get_x(a) - get_x(b);
    double yd = get_y(a) - get_y(b);

    double dij = sqrt(xd * xd + yd * yd);
    return nint(dij);
}

// --------------------------------------------
// Distance ATT (pseudo-euclidienne)
// --------------------------------------------
double dist_att(tInstance a, tInstance b) {
    double xd = get_x(a) - get_x(b);
    double yd = get_y(a) - get_y(b);

    double rij = sqrt((xd * xd + yd * yd) / 10.0);
    int tij = nint(rij);

    int dij;
    if (tij < rij)
        dij = tij + 1;
    else
        dij = tij;

    return dij;
}

// --------------------------------------------
// Distance géographique (GEO)
// --------------------------------------------

static double geo_to_rad(double coord) {
    int deg = (int)(coord);       
    double min = coord - deg;     
    return PI * (deg + 5.0 * min / 3.0) / 180.0;
}

// Calcul de la distance géographique entre deux instances
double dist_geo(tInstance a, tInstance b) {
    double lat1 = geo_to_rad(get_x(a));
    double lon1 = geo_to_rad(get_y(a));
    double lat2 = geo_to_rad(get_x(b));
    double lon2 = geo_to_rad(get_y(b));

    double q1 = cos(lon1 - lon2);
    double q2 = cos(lat1 - lat2);
    double q3 = cos(lat1 + lat2);

    int dij = (int) ( RRR * acos( 0.5*((1.0+q1)*q2 - (1.0-q1)*q3) ) + 1.0);

    return dij;
}

double tour_length(tTournee tour, DistanceFunc dist) {
    if (!tour) return 0.0;

    int total = 0.0;
    int i = 0;
    tInstance current, next;

    while ((current = get_instance_at(tour, i)) != NULL &&
           (next = get_instance_at(tour, i+1)) != NULL) {
        total += dist(current, next);
        i++;
    }

    // fermer le tour
    tInstance first = get_instance_at(tour, 0);
    tInstance last = get_instance_at(tour, i);
    if (first && last) total += dist(last, first);

    return total;
}

void canonical(void ** tour, DistanceFuncGenerique dist, int * best, double * distmin, int lenght){
    (*distmin)=0.0;
    for(int i = 0;i<lenght-1;i++){
        (*distmin)+=dist(tour[i],tour[i+1]);
        best[i] = i+1;
    }
    (*distmin)+=dist(tour[0],tour[lenght-1]);

}