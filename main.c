#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "struct.h"
#include "tspReader.h"
#include "distance.h"
#include "demi_matrice.h"

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage: %s -f fichier.tsp [-c]\n", argv[0]);
        return 1;
    }

    char *filename = NULL;
    for (int i = 1; i < argc; i++)
        if (strcmp(argv[i], "-f") == 0 && i+1 < argc)
            filename = argv[++i];

    if (!filename) { fprintf(stderr, "Erreur: aucun fichier .tsp fourni.\n"); return 1; }

    tProbleme problem = load_problem(filename);
    if (!problem) { fprintf(stderr, "Erreur: impossible de charger le fichier TSPLIB.\n"); return 1; }

    tTournee tour = get_nodes(problem);
    if (!tour) { delete_problem(&problem); return 1; }

    const char *etype = get_edge_weight_type(problem);
    DistanceFunc dist_func = dist_eucl2d;
    if (etype) {
        if (strcasecmp(etype, "ATT") == 0)dist_func = dist_att;
        else if (strcasecmp(etype, "GEO") == 0) dist_func = dist_geo;
    }

    int nb_villes = get_tour_size(problem);

    // -----------------------------
    // 1) Calcul direct avec DistanceFunc
    // -----------------------------
    
    clock_t start1 = clock();
    double length_direct = tour_length(tour, dist_func);
    clock_t end1 = clock();
    double cpu_direct = (double)(end1 - start1) / CLOCKS_PER_SEC;

    printf("Tour %s canonical %.6f %.2f [", filename, cpu_direct, length_direct);
    for (int i = 0; i < nb_villes; i++) {
        tInstance inst = get_instance_at(tour, i);
        printf("%d", get_id(inst));
        if (i < nb_villes - 1) printf(",");
    }
    printf("]\n");
    
    // -----------------------------
    // 2) Calcul via demi-matrice
    // -----------------------------
    DemiMatrice *matrice = demi_matrice_from_tour(tour, nb_villes, dist_func);
    if (!matrice) { delete_problem(&problem); return 1; }

    clock_t start2 = clock();
    double length_demi = tour_length_from_demi_matrice(tour, matrice);
    clock_t end2 = clock();
    double cpu_demi = (double)(end2 - start2) / CLOCKS_PER_SEC;

    printf("Tour %s canonical %.6f %.2f [", filename, cpu_demi, length_demi);
    for (int i = 0; i < nb_villes; i++) {
        tInstance inst = get_instance_at(tour, i);
        printf("%d", get_id(inst));
        if (i < nb_villes - 1) printf(",");
    }
    printf("]\n");

    // liberation de ressources
    detruire_demi_matrice(matrice);
    delete_problem(&problem);
    return 0;
}



