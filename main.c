#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "struct.h"
#include "tspReader.h"
#include "distance.h"
#include "demi_matrice.h"
#include "signal_handler.h"
#include "bruteforce.h"

void usage(char * arg){
    printf("Usage : %s -f fichier.tsp [-c] [-bf] [-bfm]\n",arg);
    printf("  -f : nom du fichier TSPLIB à lire\n");
    printf("  -c : (optionnel) calculer la longueur de la tournée canonique\n");
    printf("  -bf : (optionnel) calculer la longeur optimale par force brute\n");
    printf("  -bfm : (optionnel) calculer la longeur optimale par force brute matricielle (pas encore implémenté)\n");
    printf("  -h : help, affiche l'usage et ne fait aucun calcul.\n");
}

DistanceFunc dist_Code_Func(int dist_code){
    switch(dist_code){
        case 1:
            return dist_att;
        break;
        case 2:
            return dist_geo;
        break;
        default:
            return dist_eucl2d;
        break;
    }
}


int main(int argc, char *argv[]) {

    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    char *filename = NULL;
    int iscanonic=0,bf=0,bfm=0;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            filename = argv[++i];
        }
        if (strcmp(argv[i], "-c") == 0) {
            iscanonic = 1;
        }
        if (strcmp(argv[i], "-bf") == 0) {
            bf = 1;
        }
        if (strcmp(argv[i], "-bfm") == 0) {
            bfm = 1;
        }
        if (strcmp(argv[i], "-h") == 0) {
            usage(argv[0]);
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
    const char *etype = get_edge_weight_type(problem);
    int dist_code = 0;  // 0 = EUCL_2D, 1 = ATT, 2 = GEO
    if (etype) {
        if (strcasecmp(etype, "ATT") == 0) dist_code = 1;
        else if (strcasecmp(etype, "GEO") == 0) dist_code = 2;
    }
    if(iscanonic){
        // Chronométrer le calcul
        clock_t start = clock();

        double length = 0.0;
        length = tour_length(tour,dist_Code_Func(dist_code));

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
    }
    if(bf || bfm){

        //TEST BRUTEFORCE
        int bff;
        if(bf){
            bff = 0;
        }else{
            bff = 1; // bff = 1 mais erreur de segmentation
        }
        int * best = malloc(sizeof(int)*get_taille_tournee(tour));
        double dist;
        printf("Calcul des distances (Ctrl+C pour interruption)...\n");

        // Setup Ctrl+C
        setup_signal_handler(tour,dist_Code_Func(dist_code),best,&dist,bff);
        
        printf("dist = %lf\n",dist);
        printf("[");

        for(int i = 0;i<get_taille_tournee(tour)-1;i++){
            printf("%d,",best[i]);
        }
        printf("%d]\n",best[get_taille_tournee(tour)-1]);
        free(best);
    }
    




    // Libération memoire

    delete_problem(&problem);
    return 0;
}


