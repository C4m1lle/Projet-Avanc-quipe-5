/**
 * @file signal_handler.c
 * @brief Gestion des signaux pour permettre l'interruption contrôlée du bruteforce.
 */

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "../tsp/struct.h"
#include "demi_matrice.h"
#include "../distance/distance.h"
#include "signal_handler.h"
#include "../tsp/tspReader.h"
#include "bruteforce.h"

/**
 * @brief Variables statiques accessibles au signal handler.
 */
static tTournee static_path = NULL;
static double * current_distmin = NULL;
static int * current_path = NULL;
static int path_lenght = 0;
static DistanceFunc current_dist = NULL;
static int has_to_stop = 0;

/**
 * @brief Affiche la permutation courante.
 *
 * @param current_path Tableau représentant le chemin courant.
 */
static void print_tour(int * current_path) {
    printf("[");
    int i;
    for (i = 0; i < path_lenght-1; i++) {
        printf("%d,", current_path[i]);
    }
    printf("%d]\n",current_path[i]);
}

/**
 * @brief Signal handler pour l'interruption Ctrl+C.
 *
 * Permet d'afficher l'état courant du calcul et de demander à l'utilisateur
 * s'il souhaite arrêter ou continuer l'exécution.
 *
 * @param sig Signal reçu.
 */
static void traiterSignal(int sig) {
    (void)sig;
    printf("\n=== Interruption détectée (Ctrl+C) ===\n");
    
    printf("Longueur actuelle du tour : %.2lf\n", *current_distmin);
    printf("Permutation courante : ");
    print_tour(current_path);
    

    printf("Voulez-vous arrêter le programme ? (o/n) : ");
    char c;
    do{
        c = getchar();
    }while(c != 'o' && c != 'O' && c != 'n' && c != 'N');
    if (c == 'o' || c == 'O') {
        printf("Libération de la mémoire et arrêt du programme...\n");
        
        has_to_stop = 1;
    }
    if (c == 'n' || c == 'N') {
        printf("Reprise du calcul...\n");
    }
}

/**
 * @brief Initialise le gestionnaire de signal pour l'algorithme de bruteforce.
 *
 * Cette fonction configure le handler pour le signal SIGINT (Ctrl+C),
 * puis lance l'algorithme de bruteforce, soit en version classique, soit
 * avec la demi-matrice selon le paramètre fourni.
 *
 * @param tour Tournée initiale.
 * @param dist Fonction de calcul de distance.
 * @param best Tableau stockant la meilleure permutation.
 * @param distmin Pointeur vers la distance minimale trouvée.
 * @param bff Booléen indiquant le choix de la version du bruteforce (0 = classique, 1 = demi-matrice).
 */
void setup_signal_handler(tTournee tour, DistanceFunc dist,int * best, double * distmin, int bff) {
    static_path = tour;
    current_distmin = distmin;
    current_path = best;
    current_dist = dist;
    path_lenght = get_taille_tournee(tour);

    struct sigaction action;
    sigset_t mask;
    action.sa_handler = traiterSignal;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGINT, &action, NULL) != 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    sigemptyset(&mask);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) != 0) {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    if(!bff){
        bruteforce((void **)get_chemin_tournee(tour),(DistanceFuncGenerique)dist,best,distmin,get_taille_tournee(tour),&has_to_stop);
    }else{
        bruteforce_demi_matrice((void**)get_chemin_tournee(tour),(DistanceFuncGenerique)dist,best,distmin,get_taille_tournee(tour),&has_to_stop);
    }
}
