#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "rw.h"
#include "nn.h"
#include "struct.h"
#include "tspReader.h"
#include "distance.h"
#include "demi_matrice.h"
#include "signal_handler.h"
#include "bruteforce.h"
#include "opt2.h"

#define BF_GA 200
#define BFM 309
#define NN 220
#define RW 233
#define DEUXOPTNN 609
#define DEUXOPTRW 622
#define GADPX 532
#define ALL 313


void usage(char * arg){
    printf("Usage : %s [<-f file.tsp> [-o <output.txt>] [-c] [-d {eucl2d | att | geo}] [-m {bf | bfm | nn | rw | 2optnn | 2optrw | ga}]] [-h]\n",arg);
    printf("  -f : nom du fichier TSPLIB à lire\n");
    printf("  -o <output.txt> : rediriger la sortie vers un fichier .txt\n");
    printf("  -d <distance_type> :  choix de la distance choisies pour les calculs (eucl2d par défaut)\n");
    printf("  -c : afficher la longueur de la tournée canonique\n");
    printf("  -m {bf | bfm | nn | rw | 2optnn | 2optrw | ga} : recherche de la longueur optimale selon la méthode choisie\n");
    printf("                                         bf : force brute\n");
    printf("                                         bfm : force brute matricielle\n");
    printf("                                         nn : plus proche voisin (nearest neighbor)\n");
    printf("                                         rw : marche aléatoire (random walk)\n");
    printf("                                         ga <nombre d'individus> <nombre de générations> <taux de mutation> : algorithme génétique générique\n");
    printf("  -h : help, affiche l'usage et ne fait aucun calcul.\n");
}


void affichage_test_python(FILE * output_file,char * filename, char * method, double sec, double length, int * tournee, int taille_tournee){
    
    fprintf(output_file,"%s %s %.6f %.2f [", filename, method, sec, length);

    for(int i = 0; i < taille_tournee-1; i++){
        fprintf(output_file,"%d,",tournee[i]);
    }
    fprintf(output_file,"%d]\n",tournee[taille_tournee-1]);
}




int main(int argc, char *argv[]) {

    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }
    
    char *filename = NULL;
    FILE * output_file = stdout;
    char mMode_buffer[7];
    int iscanonic=0,bf=0,bfm=0,nn=0,rw=0,deux_optnn=0,deux_optrw=0,ga=0,gadpx=0,m=0,j,sum,force_dist_method=0;
    DistanceFunc dist_method = dist_eucl2d;
    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-f") == 0) {
            if(i + 1 >= argc){
                usage(argv[0]);
                return 1;
            }
            filename = argv[++i];
        }
        if(strcmp(argv[i], "-o") == 0) {
            if(i + 1 >= argc){
                usage(argv[0]);
                return 1;
            }
            if((output_file = fopen(argv[i+1],"a")) == NULL){
                fprintf(stderr,"Fichier %s inaccessible ou inexistant.\n",argv[++i]);
                return 1;
            }
            
        }
        if (strcmp(argv[i], "-c") == 0) {
            iscanonic = 1;
        }
        if (strcmp(argv[i], "-d") == 0) {
            if(i+1 >= argc){
                usage(argv[0]);
                return 1;
            }
            if (strcmp(argv[i+1], "att") == 0){
                dist_method = dist_att;
            }
            if (strcmp(argv[i+1], "geo") == 0){
                dist_method = dist_geo;
            }
            force_dist_method=1;
        }
        if (strcmp(argv[i], "-m") == 0) {
            if( (m==1) | (i+1 >= argc)){
                usage(argv[0]);
                return 1;
            }
            m=1; sum=0; j=0;
            strcpy(mMode_buffer,argv[i+1]);
            while(mMode_buffer[j]!='\0' && j < 7){
                sum+=mMode_buffer[j];
                j++;
            }
            switch(sum){
                case BF_GA:// somme ascii de ga | bf
                    if (strcmp(mMode_buffer, "bf") == 0){
                        bf = 1;
                    }else{
                        ga = 1;
                    }
                break;
                case BFM:
                    bfm = 1;
                break;
                case NN:
                    nn = 1;
                break;
                case RW:
                    rw = 1;
                break;
                case DEUXOPTNN:
                    deux_optnn = 1;
                break;
                case DEUXOPTRW:
                    deux_optrw = 1;
                break;
                case GADPX:
                    gadpx = 1;
                break;
                case ALL:
                    ga = 1;
                    nn = 1;
                    rw = 1;
                    deux_optnn = 1;
                    deux_optrw = 1;
                    gadpx = 1;
                break;
                default:
                    usage(argv[0]);
                    return 1;
                break;

            }
        }
        if (strcmp(argv[i], "-h") == 0){
            usage(argv[0]);
            return 0;
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
    int taille_Tournee = get_taille_tournee(tour);
    if (etype && !force_dist_method) {
        if (strcasecmp(etype, "ATT") == 0) dist_method = dist_att;
        else if (strcasecmp(etype, "GEO") == 0) dist_method = dist_geo;
    }
    fprintf(output_file,"Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");
    if(iscanonic){
        // Chronométrer le calcul
        clock_t start = clock();

        double length = 0.0;
        length = tour_length(tour,dist_method);

        clock_t end = clock();
        double cpu_time = (double)(end - start) / CLOCKS_PER_SEC;

        // Affichage au format Python attendu

        int * tab_tournee = malloc(sizeof(int)*taille_Tournee);
        tInstance inst;

        for(int id = 0; id<taille_Tournee; id++){
            inst = get_instance_at(tour, id);
            tab_tournee[id] = get_id(inst);
        }

        affichage_test_python(output_file,filename, "canonical", cpu_time, length, tab_tournee, taille_Tournee);
        free(tab_tournee);
    }
    if(bf){

        //TEST BRUTEFORCE
        int * best_bf = malloc(sizeof(int)*taille_Tournee);
        double dist;
        //printf("Calcul des distances (Ctrl+C pour interruption)...\n");

        // Setup Ctrl+C
        clock_t startbf = clock();
        setup_signal_handler(tour,dist_method,best_bf,&dist,0);
        clock_t endbf = clock();
        double bf_time = (double)(endbf - startbf) / CLOCKS_PER_SEC;
        affichage_test_python(output_file,filename, "bf", bf_time, dist, best_bf, taille_Tournee);
        free(best_bf);
    }
    if(bfm){

        //TEST BRUTEFORCE
        int * best_bfm = malloc(sizeof(int)*taille_Tournee);
        double dist;
        //printf("Calcul des distances (Ctrl+C pour interruption)...\n");

        // Setup Ctrl+C
        clock_t startbfm = clock();
        setup_signal_handler(tour,dist_method,best_bfm,&dist,1);
        clock_t endbfm = clock();
        double bfm_time = (double)(endbfm - startbfm) / CLOCKS_PER_SEC;
        affichage_test_python(output_file,filename, "bfm", bfm_time, dist, best_bfm, taille_Tournee);
        free(best_bfm);
    }
     /* --- Random Walk (rw) --- */
    if (rw) {
        char method[4];
        sprintf(method, "rw");

        int n = taille_Tournee;
        int *best = malloc(sizeof(int) * n);
        if (!best) {
            fprintf(stderr, "Erreur memoire allocation (rw)\n");
        } else {
            double dist_found = 0.0;
            //printf("Exécution de la marche aléatoire (random walk)...\n");
            clock_t startrw = clock();

            /* Appel de la fonction random_walk :
             * int random_walk(tTournee tour, DistanceFunc dist, int *bestTour, double *bestDist);
             */
            int rc = random_walk(tour, dist_method, best, &dist_found);

            clock_t endrw = clock();
            double rw_time = (double)(endrw - startrw) / CLOCKS_PER_SEC;

            if (rc != 0) {
                fprintf(stderr, "random_walk a retourné une erreur (rc=%d)\n", rc);
            } else {
                affichage_test_python(output_file,filename, method, rw_time, dist_found, best, n);
            }

            free(best);
        }
    }
    if (nn) {
        char method[3];
        sprintf(method, "nn");

        int *best = malloc(sizeof(int) * taille_Tournee);
            if (!best) {
                fprintf(stderr, "Erreur memoire allocation (nn)\n");
            } else {
                double dist_found = 0.0;

                clock_t startnn = clock();
                plus_proche_voisin(tour, dist_method, best, &dist_found);

                clock_t endnn = clock();
                double nn_time = (double)(endnn - startnn) / CLOCKS_PER_SEC;

                affichage_test_python(output_file,filename, method, nn_time, dist_found, best, taille_Tournee);

                free(best);
        }
    }
    if(deux_optnn){
        char method[7];
        sprintf(method, "opt2nn");

        int *best = malloc(sizeof(int) * taille_Tournee);
            if (!best) {
                fprintf(stderr, "Erreur memoire allocation (opt2nn)\n");
            } else {
                double dist_found = 0.0;

                clock_t startOptnn = clock();
                plus_proche_voisin(tour, dist_method, best, &dist_found);
                dist_found = opt2(dist_method,tour,best);
                clock_t endOptnn = clock();
                double Optnn_time = (double)(endOptnn - startOptnn) / CLOCKS_PER_SEC;

                affichage_test_python(output_file,filename, method, Optnn_time, dist_found, best, taille_Tournee);

                free(best);
        }
    }
    if (deux_optrw) {
        char method[7];
        sprintf(method, "opt2rw");

        int n = taille_Tournee;
        int *best = malloc(sizeof(int) * n);
        if (!best) {
            fprintf(stderr, "Erreur memoire allocation (opt2rw)\n");
        } else {
            double dist_found = 0.0;
            //printf("Exécution de la marche aléatoire (random walk)...\n");
            clock_t startOptrw = clock();

            /* Appel de la fonction random_walk :
             * int random_walk(tTournee tour, DistanceFunc dist, int *bestTour, double *bestDist);
             */
            int rc = random_walk(tour, dist_method, best, &dist_found);
            dist_found = opt2(dist_method,tour,best);
            clock_t endOptrw = clock();
            double Optrw_time = (double)(endOptrw - startOptrw) / CLOCKS_PER_SEC;

            if (rc != 0) {
                fprintf(stderr, "random_walk a retourné une erreur (rc=%d)\n", rc);
            } else {
                affichage_test_python(output_file,filename, method, Optrw_time, dist_found, best, n);
            }

            free(best);
        }
    }


    // Libération memoire

    delete_problem(&problem);
    if(output_file!=stdout){
        fclose(output_file);
    }
    return 0;
}


