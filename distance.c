#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "distance.h"
#include "struct.h"
// --------------------------------------------
// Distance EUCLIDIENNE (EUCL_2D)
// --------------------------------------------
double dist_eucl2d(tInstance a, tInstance b) {
    double dx = b->x - a->x;
    double dy = b->y - a->y;
    return sqrt(dx * dx + dy * dy);
}

// --------------------------------------------
// Distance ATT (pseudo-euclidienne TSPLIB)
// --------------------------------------------
double dist_att(tInstance a, tInstance b) {
    double dx = b->x - a->x;
    double dy = b->y - a->y;
    double value = sqrt((dx * dx + dy * dy) / 10.0);
    double distance = round(value);
    if (distance < value) distance += 1.0;
    return distance;
}

// --------------------------------------------
// Distance géographique (GEO - TSPLIB)
// --------------------------------------------
static double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

double dist_geo(tInstance a, tInstance b) {
    double radius = 6378.388; // rayon terrestre (km)
    double lat1 = deg2rad(a->x);
    double lon1 = deg2rad(a->y);
    double lat2 = deg2rad(b->x);
    double lon2 = deg2rad(b->y);

    double q1 = cos(lon1 - lon2);
    double q2 = cos(lat1 - lat2);
    double q3 = cos(lat1 + lat2);
    double distance = radius * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0;
    return distance;
}

// --------------------------------------------
// Calcul de la longueur d’une tournée complète
// --------------------------------------------
double tour_length(tTournee tour, DistanceFunc dist) {
    if (!tour || tour->taille<2) return 0.0;

    double total = 0.0;
    for (int i = 0; i < tour->current - 1; i++) {
        total += dist(tour->chemin[i], tour->chemin[i + 1]);
    }

    // Retour à la ville de départ
    total += dist(tour->chemin[tour->current - 1], tour->chemin[0]);
    return total;
}

int main(void) {
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

    return 0;
}

