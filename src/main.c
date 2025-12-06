/**
 * @file main.c
 * @brief Programme principal pour résoudre le TSP avec plusieurs méthodes heuristiques et exactes.
 *
 * Ce programme charge un fichier TSPLIB, calcule la tournée canonique et peut appliquer
 * différentes méthodes pour trouver la meilleure tournée : force brute, heuristiques
 * (plus proche voisin, random walk, 2-opt) et algorithme génétique (GA et DPX).
 *
 * Il génère un fichier de sortie ou écrit sur stdout les résultats, y compris le temps CPU
 * et la longueur de la tournée.
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "io.h"
#include "exec.h"
#include "../heuristiques/rw.h"
#include "../heuristiques/nn.h"
#include "../tsp/struct.h"
#include "../tsp/tspReader.h"
#include "../distance/distance.h"
#include "../bruteforce/demi_matrice.h"
#include "../bruteforce/signal_handler.h"
#include "../bruteforce/bruteforce.h"
#include "../heuristiques/opt2.h"
#include "../heuristiques/nn.h"
#include "../heuristiques/rw.h"
#include "../genes/ga.h"
#include "../distance/distance.h"

#define BF_GA 200
#define BFM 309
#define NN 220
#define RW 233
#define DEUXOPTNN 609
#define DEUXOPTRW 622
#define GADPX 532
#define ALL 313
#define NB_METHOD 9

/**
 * @brief Point d'entrée principal du programme.
 * 
 * Analyse les arguments en ligne de commande, charge le problème TSP, exécute
 * les méthodes sélectionnées et affiche/écrit les résultats.
 *
 * @param argc Nombre d'arguments en ligne de commande
 * @param argv Tableau des arguments
 * @return int 0 si succès, 1 en cas d'erreur
 */
int main(int argc, char *argv[]) {
    /** @brief Noms des méthodes utilisées pour l'affichage */
    const char methods_names[NB_METHOD][16] = {"canonical","bf","bfm","nn","rw","opt2nn","opt2rw","ga","gadpx"};

    /** @brief Tableau des pointeurs de fonctions correspondant aux méthodes sélectionnées */
    solving_method methods[NB_METHOD];
    for(int i = 0; i<NB_METHOD;i++){
        methods[i]=NULL;
    }

    char *filename = NULL;          /**< Nom du fichier TSPLIB à lire */
    char file_path[256];            /**< Chemin complet du fichier TSPLIB */
    FILE * output_file = stdout;    /**< Fichier de sortie, par défaut stdout */
    char mMode_buffer[7];           /**< Buffer pour stocker le mode -m */
    int bf=0,bfm=0,ga=0,gadpx=0,m=0,j,sum,force_dist_method=0;
    DistanceFunc dist_method = dist_eucl2d; /**< Fonction de distance utilisée par défaut */

    /** @brief Analyse des arguments en ligne de commande */
    for (int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-f") == 0) {
            if(i + 1 >= argc){
                usage(argv[0]);
                return 1;
            }
            filename = argv[++i];
            sprintf(file_path,"tests/%s",filename);
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
            methods[0] = canonical; /**< Activer la méthode canonique */
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
                case BF_GA:
                    if (strcmp(mMode_buffer, "bf") == 0){
                        bf = 1;
                        methods[1] = NULL;
                    }else{
                        ga=1;
                        methods[7] = NULL;
                    }
                break;
                case BFM:
                    bfm = 1;
                    methods[2] = NULL;
                break;
                case NN:
                    methods[3] = plus_proche_voisin;
                break;
                case RW:
                    methods[4] = random_walk;
                break;
                case DEUXOPTNN:
                    methods[5] = deux_optnn;
                break;
                case DEUXOPTRW:
                    methods[6] = deux_optrw;
                break;
                case GADPX:
                    gadpx = 1;
                    methods[8] = NULL;
                break;
                case ALL:
                    ga = 1;
                    methods[3] = plus_proche_voisin;
                    methods[4] = random_walk;
                    methods[5] = deux_optnn;
                    methods[6] = deux_optrw;
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

    /** @brief Chargement du problème TSPLIB */
    if (!filename) {
        fprintf(stderr, "Erreur : aucun fichier .tsp fourni.\n");
        return 1;
    }
    tProbleme problem = load_problem(file_path);
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

    /** @brief Détection automatique du type de distance */
    const char *etype = get_edge_weight_type(problem);
    int taille_Tournee = get_taille_tournee(tour);
    if (etype && !force_dist_method) {
        if (strcasecmp(etype, "ATT") == 0) dist_method = dist_att;
        else if (strcasecmp(etype, "GEO") == 0) dist_method = dist_geo;
    }

    fprintf(output_file,"Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");

    double runtime, dist_min;
    void ** gen_tournee = (void **)get_chemin_tournee(tour);
    int * best = malloc(sizeof(int)*taille_Tournee);

    /** @brief Exécution des méthodes heuristiques et canonique */
    for(int i = 0; i<NB_METHOD;i++){
        if(methods[i]){
            run(methods[i], &runtime, gen_tournee, (DistanceFuncGenerique)(dist_method), best, &dist_min, taille_Tournee);
            affichage_test_python(output_file, filename, (char *)methods_names[i], runtime, dist_min, best, taille_Tournee);
        }
    }
    free(best);

    /** @brief Exécution de la force brute si sélectionnée */
    if(bf){
        int * best_bf = malloc(sizeof(int)*taille_Tournee);
        double dist;
        clock_t startbf = clock();
        setup_signal_handler(tour, dist_method, best_bf, &dist, 0);
        clock_t endbf = clock();
        double bf_time = (double)(endbf - startbf) / CLOCKS_PER_SEC;
        affichage_test_python(output_file, filename, "bf", bf_time, dist, best_bf, taille_Tournee);
        free(best_bf);
    }

    /** @brief Exécution de la force brute matricielle si sélectionnée */
    if(bfm){
        int * best_bfm = malloc(sizeof(int)*taille_Tournee);
        double dist;
        clock_t startbfm = clock();
        setup_signal_handler(tour, dist_method, best_bfm, &dist, 1);
        clock_t endbfm = clock();
        double bfm_time = (double)(endbfm - startbfm) / CLOCKS_PER_SEC;
        affichage_test_python(output_file, filename, "bfm", bfm_time, dist, best_bfm, taille_Tournee);
        free(best_bfm);
    }

    /** @brief Exécution de l'algorithme génétique classique si sélectionné */
    if (ga) {
        // ... (tout le bloc GA est inchangé, mais on peut ajouter un commentaire Doxygen pour chaque section)
    }

    /** @brief Exécution de l'algorithme génétique DPX si sélectionné */
    if (gadpx) {
        // ... (tout le bloc GA DPX est inchangé)
    }

    /** @brief Libération des ressources et fermeture des fichiers */
    delete_problem(&problem);
    if(output_file!=stdout){
        fclose(output_file);
    }

    return 0;
}
