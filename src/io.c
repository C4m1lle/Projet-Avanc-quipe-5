#include <stdlib.h>
#include <stdio.h>
#include "./distance/distance.h"
#include "io.h"

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




int param(int argc, char *argv[]){

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
}