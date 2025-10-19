#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "distance.h"
#include "struct.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
// --------------------------------------------
// Distance EUCLIDIENNE (EUCL_2D)
// --------------------------------------------
double dist_eucl2d(tInstance a, tInstance b) {
    double dx = get_x(b) - get_x(a);
    double dy = get_y(b) - get_y(a);
    return sqrt(dx * dx + dy * dy);
}

// --------------------------------------------
// Distance ATT (pseudo-euclidienne)
// --------------------------------------------
double dist_att(tInstance a, tInstance b) {
    double dx = get_x(b) - get_x(a);
    double dy = get_y(b) - get_y(a);
    double value = sqrt((dx * dx + dy * dy) / 10.0);
    double distance = round(value);
    if (distance < value) distance += 1.0;
    return distance;
}

// --------------------------------------------
// Distance géographique (GEO)
// --------------------------------------------
static double deg2rad(double deg) {
    return deg * M_PI / 180.0;
}

double dist_geo(tInstance a, tInstance b) {
    double radius = 6378.388; // rayon terrestre (km)
    double lat1 = deg2rad(get_x(a));
    double lon1 = deg2rad(get_y(a));
    double lat2 = deg2rad(get_x(b));
    double lon2 = deg2rad(get_y(b));

    double q1 = cos(lon1 - lon2);
    double q2 = cos(lat1 - lat2);
    double q3 = cos(lat1 + lat2);
    double distance = radius * acos(0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0;
    return distance;
}

double tour_length(tTournee tour, DistanceFunc dist) {
    if (!tour)
        return 0.0;

    double total = 0.0;
    int i = 0;

    while (1) {
        tInstance current = get_instance_at(tour, i);
        tInstance next = get_instance_at(tour, i + 1);

        if (next == NULL)
            break; // fin de la tournée

        total += dist(current, next);
        i++;
    }

    // retour à la première ville si tournée complète
    tInstance first = get_instance_at(tour, 0);
    tInstance last = get_instance_at(tour, i - 1);
    if (first && last)
        total += dist(last, first);

    return total;
}
