#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include "struct.h"
#include "tspReader.h"
#include "distance.h"
#include "bruteforce.h"

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Usage : %s -f fichier.tsp [-c]\n", argv[0]);
        printf("  -f : nom du fichier TSPLIB à lire\n");
        printf("  -c : (optionnel) calculer la longueur de la tournée canonique\n");
        return 1;
    }

    char *filename = NULL;
    int iscanonic = 1;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        }
        if (strcmp(argv[i], "-c") == 0) {
            iscanonic = 0;
        }
    }

    if (!filename) {
        fprintf(stderr, "Erreur : aucun fichier .tsp fourni.\n");
        return 1;
    }

    tProbleme problem = load_problem(filename);
    if (!problem) {
        fprintf(stderr, "Erreur : impossible de charger le fichier TSPLIB.\n");
        return 1;
    }

    tTournee tour = get_nodes(problem);
    if (!tour) {
        fprintf(stderr, "Erreur : aucune tournée trouvée dans le fichier.\n");
        delete_problem(&problem);
        return 1;
    }

    //TEST BRUTEFORCE
    int * best = malloc(sizeof(int)*get_taille_tournee(tour));
    printf("dist = %lf\n",bruteforce(tour,dist_eucl2d,best));
    printf("[");
    for(int i = 0;i<get_taille_tournee(tour)-1;i++){
        printf("%d, ",best[i]);
    }
    printf("%d]\n",best[get_taille_tournee(tour)-1]);


    const char *etype = get_edge_weight_type(problem);
    int dist_code = 0;  // 0 = EUCL_2D, 1 = ATT, 2 = GEO
    if (etype) {
        if (strcasecmp(etype, "ATT") == 0) dist_code = 1;
        else if (strcasecmp(etype, "GEO") == 0) dist_code = 2;
    }

    // Chronométrer le calcul
    clock_t start = clock();

    double length = 0.0;
    if (dist_code == 1)
        length = tour_length(tour, dist_att);
    else if (dist_code == 2)
        length = tour_length(tour, dist_geo);
    else
        length = tour_length(tour, dist_eucl2d);

    clock_t end = clock();
    double cpu_time = (double)(end - start) / CLOCKS_PER_SEC;

    // Affichage au format Python attendu

    printf("Tour %s canonical %.6f %.2f [", filename, cpu_time, length);

    int i = 0;
    tInstance inst;
    while ((inst = get_instance_at(tour, i)) != NULL) {
        printf("%d", get_id(inst));
        i++;
        if (get_instance_at(tour, i) != NULL)
            printf(",");
    }
    printf("]\n");

    delete_problem(&problem);



    return 0;
}


