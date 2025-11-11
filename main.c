#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>

#include "nn.h"
#include "struct.h"
#include "tspReader.h"
#include "distance.h"
#include "demi_matrice.h"
#include "signal_handler.h"
#include "bruteforce.h"

#define BF_GA 200
#define BFM 309
#define NN 220
#define RW 233
#define DEUXOPTNN 609
#define DEUXOPTRW 622


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

void affichage_test_python(char * filename, char * method, double sec, double length, int * tournee, int taille_tournee){
    printf("%s ; %s ; %.6f ; %.2f ; [", filename, method, sec, length);
    for(int i = 0; i < taille_tournee-1; i++){
        printf("%d, ",tournee[i]);
    }
    printf("%d]\n",tournee[taille_tournee-1]);
}



int main(int argc, char *argv[]) {

    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }
    
    char *filename = NULL;
    char mMode_buffer[7];
    int iscanonic=0,bf=0,bfm=0,nn=0,rw=0,deux_optnn=0,deux_optrw=0,ga=0,m=0,j,sum;
    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-f") == 0) {
            if(i + 1 >= argc){
                usage(argv[0]);
                return 1;
            }
            filename = argv[++i];
        }
        if (strcmp(argv[i], "-c") == 0) {
            iscanonic = 1;
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
        char method[4];
        if(bf){
            bff = 0;
            sprintf(method,"bf");
        }else{
            bff = 1;
            sprintf(method,"bfm");
        }
        int * best = malloc(sizeof(int)*get_taille_tournee(tour));
        double dist;
        printf("Calcul des distances (Ctrl+C pour interruption)...\n");

        // Setup Ctrl+C
        clock_t startbf = clock();
        setup_signal_handler(tour,dist_Code_Func(dist_code),best,&dist,bff);
        clock_t endbf = clock();
        double bf_time = (double)(endbf - startbf) / CLOCKS_PER_SEC;
        affichage_test_python(filename, method, bf_time, dist, best, get_taille_tournee(tour));
    }
    




    // Libération memoire

    delete_problem(&problem);
    return 0;
}


