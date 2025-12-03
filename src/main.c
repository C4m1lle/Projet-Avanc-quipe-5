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


int main(int argc, char *argv[]) {
    const char methods_names[NB_METHOD][16] = {"canonical","bf","bfm","nn","rw","opt2nn","opt2rw","ga","gadpx"};
    if (argc < 2) {
        usage(argv[0]);
        return 1;
    }
    solving_method methods[NB_METHOD];
    for(int i = 0; i<NB_METHOD;i++){
        methods[i]=NULL;
    }

    char *filename = NULL;
    char file_path[256];

    FILE * output_file = stdout;
    char mMode_buffer[7];
    int bf=0,bfm=0,nn=0,rw=0,deux_optnn=0,deux_optrw=0,ga=0,gadpx=0,m=0,j,sum,force_dist_method=0;
    DistanceFunc dist_method = dist_eucl2d;
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
            methods[0] = canonical;
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
                    deux_optnn = 1;
                    methods[6] = NULL;
                break;
                case DEUXOPTRW:
                    deux_optrw = 1;
                    methods[7] = NULL;
                break;
                case GADPX:
                    gadpx = 1;
                    methods[8] = NULL;
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
    const char *etype = get_edge_weight_type(problem);
    int taille_Tournee = get_taille_tournee(tour);
    if (etype && !force_dist_method) {
        if (strcasecmp(etype, "ATT") == 0) dist_method = dist_att;
        else if (strcasecmp(etype, "GEO") == 0) dist_method = dist_geo;
    }
    fprintf(output_file,"Instance ; Méthode ; Temps CPU (sec) ; Longueur ; Tour\n");

    double runtime,dist_min;
    void ** gen_tournee = (void **)get_chemin_tournee(tour);
    int * best = malloc(sizeof(int)*taille_Tournee);
    for(int i = 0; i<NB_METHOD;i++){
        if(methods[i]){
            run(methods[i],&runtime,gen_tournee,(DistanceFuncGenerique)(dist_method),best,&dist_min,taille_Tournee);
            affichage_test_python(output_file,filename,(char *)methods_names[i],runtime,dist_min,best,taille_Tournee);
        }
    }
    free(best);

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
            random_walk((void**)get_chemin_tournee(tour), (DistanceFuncGenerique)dist_method, best, &dist_found,get_taille_tournee(tour));

            clock_t endrw = clock();
            double rw_time = (double)(endrw - startrw) / CLOCKS_PER_SEC;

            affichage_test_python(output_file,filename, method, rw_time, dist_found, best, n);


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

                double nn_time;
                run(plus_proche_voisin, &nn_time, (void **)get_chemin_tournee(tour), (DistanceFuncGenerique)dist_method, best, &dist_found,get_taille_tournee(tour));

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
                plus_proche_voisin((void **)get_chemin_tournee(tour), (DistanceFuncGenerique)dist_method, best, &dist_found,get_taille_tournee(tour));
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
            random_walk((void**)get_chemin_tournee(tour), (DistanceFuncGenerique)dist_method, best, &dist_found,get_taille_tournee(tour));
            dist_found = opt2(dist_method,tour,best);
            clock_t endOptrw = clock();
            double Optrw_time = (double)(endOptrw - startOptrw) / CLOCKS_PER_SEC;

            affichage_test_python(output_file,filename, method, Optrw_time, dist_found, best, n);


            free(best);
        }
    }
    if (ga) {

    int n = taille_Tournee;
    if (n <= 0) {
        fprintf(stderr, "Error: taille_Tournee must be > 0\n");
        exit(1);
    }

    srand((unsigned int)time(NULL));

    /* Allocation du tableau contenant les IDs du meilleur tour */
    int *best_ids = malloc(sizeof(int) * n);
    if (!best_ids) { perror("malloc best_ids"); exit(1); }

    /* Lecture des paramètres GA */
    int pop_size = 30;
    int generations = 100;
    double mutation_rate = 0.1;

    if (argc >= 8 && strcmp(argv[4], "ga") == 0) {
        pop_size      = atoi(argv[5]);
        generations   = atoi(argv[6]);
        mutation_rate = atof(argv[7]);
    }

    /* Construction du tableau des villes */
    tInstance *cities = malloc(sizeof(tInstance) * n);
    if (!cities) { perror("malloc cities"); free(best_ids); exit(1); }

    for (int i = 0; i < n; i++)
        cities[i] = get_instance_at(tour, i);

    /* Création de la structure GA_Data */
    GA_Data data;
    data.dist = dist_method;
    data.cities = cities;
    data.n = n;

    /* Création de la population initiale */
    Individual *population = malloc(sizeof(Individual) * pop_size);
    if (!population) {
        perror("malloc population");
        free(cities);
        free(best_ids);
        exit(1);
    }

    for (int i = 0; i < pop_size; i++)
        population[i] = ga_create_random_tour(n, &data);

    /* Paramètres GA */
    GA_Parameters params;
    params.population_size = pop_size;
    params.generations     = generations;
    params.mutation_rate   = mutation_rate;
    params.tournament_size = (int)(0.7 * pop_size);
    params.individual_size = n;
    params.data            = &data;

    /* Exécution du GA */
    Individual best_ind = NULL;
    double best_score = 0.0;

    clock_t start = clock();

    int rc = ga_run(
        population,
        &params,
        ga_copy_tour,
        ga_delete_tour,
        ga_mutate_tour,
        ga_ordered_crossover,
        ga_fitness_tour,
        ga_tournament_selection,
        ga_create_random_tour,
        &best_ind,
        &best_score
    );

    clock_t end = clock();
    double cpu_time = (double)(end - start) / CLOCKS_PER_SEC;

    if (rc != 0) {
        fprintf(stderr, "ga_run failed\n");
    } else if (best_ind) {

        for (int i = 0; i < n; i++)
            best_ids[i] = get_id(get_instance_at((tTournee)best_ind, i));

        affichage_test_python(
            output_file, filename, "ga",
            cpu_time, best_score, best_ids, n
        );
    }
    /* Libération */
    free(cities);
    free(best_ids);
    if (best_ind)
        delete_tournee_without_instances((tTournee*)&best_ind);
    }
    if (gadpx) {

    int n = taille_Tournee;
    if (n <= 0) {
        fprintf(stderr, "Error: taille_Tournee must be > 0\n");
        exit(1);
    }

    srand((unsigned int)time(NULL));

    /* Allocation du tableau contenant les IDs du meilleur tour */
    int *best_ids = malloc(sizeof(int) * n);
    if (!best_ids) { perror("malloc best_ids"); exit(1); }

    /* Lecture des paramètres GA */
    int pop_size = 30;
    int generations = 100;
    double mutation_rate = 0.1;

    if (argc >= 8 && strcmp(argv[4], "ga") == 0) {
        pop_size      = atoi(argv[5]);
        generations   = atoi(argv[6]);
        mutation_rate = atof(argv[7]);
    }

    /* Construction du tableau des villes */
    tInstance *cities = malloc(sizeof(tInstance) * n);
    if (!cities) { perror("malloc cities"); free(best_ids); exit(1); }

    for (int i = 0; i < n; i++)
        cities[i] = get_instance_at(tour, i);

    /* Création de la structure GA_Data */
    GA_Data data;
    data.dist = dist_method;
    data.cities = cities;
    data.n = n;

    /* Création de la population initiale */
    Individual *population = malloc(sizeof(Individual) * pop_size);
    if (!population) {
        perror("malloc population");
        free(cities);
        free(best_ids);
        exit(1);
    }

    for (int i = 0; i < pop_size; i++)
        population[i] = ga_create_random_tour(n, &data);

    /* Paramètres GA */
    GA_Parameters params;
    params.population_size = pop_size;
    params.generations     = generations;
    params.mutation_rate   = mutation_rate;
    params.tournament_size = (int)(0.7 * pop_size);
    params.individual_size = n;
    params.data            = &data;

    /* Exécution du GA */
    Individual best_ind = NULL;
    double best_score = 0.0;

    clock_t start = clock();

    int rc = ga_run(
        population,
        &params,
        ga_copy_tour,
        ga_delete_tour,
        ga_mutate_tour,
        ga_dpx_crossover,
        ga_fitness_tour,
        ga_tournament_selection,
        ga_create_random_tour,
        &best_ind,
        &best_score
    );

    clock_t end = clock();
    double cpu_time = (double)(end - start) / CLOCKS_PER_SEC;

    if (rc != 0) {
        fprintf(stderr, "ga_run failed\n");
    } else if (best_ind) {

        for (int i = 0; i < n; i++)
            best_ids[i] = get_id(get_instance_at((tTournee)best_ind, i));

        affichage_test_python(
            output_file, filename, "dpx",
            cpu_time, best_score, best_ids, n
        );
    }
    /* Libération */
    free(cities);
    free(best_ids);
    if (best_ind)
        delete_tournee_without_instances((tTournee*)&best_ind);
    } 
    // Libération memoire

    delete_problem(&problem);
    if(output_file!=stdout){
        fclose(output_file);
    }
    return 0;
}

