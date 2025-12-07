/**
 * @file io.c
 * @brief Fonctions d'entrée/sortie pour le TSP.
 *
 * Contient les fonctions pour afficher l'usage de l'application et pour
 * produire des sorties formatées, compatibles avec Python ou pour fichier texte.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../distance/distance.h"
#include "io.h"
#include "../tsp/types.h"

#define BF_GA 200
#define BFM 309
#define NN 220
#define RW 233
#define DEUXOPTNN 609
#define DEUXOPTRW 622
#define GADPX 532
#define ALL 313

/**
 * @brief Affiche l'usage de l'application TSP.
 *
 * Cette fonction est appelée lorsque l'utilisateur fournit des arguments
 * invalides ou demande l'aide (-h). Elle affiche les différentes options
 * possibles pour exécuter le programme.
 *
 * @param arg Nom de l'exécutable (argv[0]).
 */
void usage(char * arg){
    printf("Usage : %s [<-f file.tsp> [-o <output.txt>] [-c] [-d {eucl2d | att | geo}] [-m {bf | bfm | nn | rw | 2optnn | 2optrw | ga}]] [-h]\n",arg);
    printf("  -f : nom du fichier TSPLIB à lire\n");
    printf("  -o <output.txt> : rediriger la sortie vers un fichier .txt\n");
    printf("  -d <distance_type> :  choix de la distance choisies pour les calculs (eucl2d par défaut)\n");
    printf("  -c : afficher la longueur de la tournée canonique\n");
    printf("  -m {bf | bfm | nn | rw | 2optnn | 2optrw | ga | all} : recherche de la longueur optimale selon la méthode choisie\n");
    printf("                                         bf : force brute\n");
    printf("                                         bfm : force brute matricielle\n");
    printf("                                         nn : plus proche voisin (nearest neighbor)\n");
    printf("                                         rw : marche aléatoire (random walk)\n");
    printf("                                         ga <nombre d'individus> <nombre de générations> <taux de mutation> : algorithme génétique générique\n");
    printf("                                         all : Toutes les méthodes sauf bf et bfm\n");
    printf("  -h : help, affiche l'usage et ne fait aucun calcul.\n");
}

/**
 * @brief Affiche les résultats pour Python ou fichier texte.
 *
 * Produit une sortie au format :
 * filename method runtime distance [tour]
 *
 * @param output_file    Fichier de sortie (stdout ou fichier .txt).
 * @param filename       Nom du fichier TSP traité.
 * @param method         Nom de la méthode utilisée (bf, nn, rw, etc.).
 * @param sec            Temps d'exécution en secondes.
 * @param length         Distance totale de la tournée calculée.
 * @param tournee        Tableau des IDs des villes dans l'ordre de la tournée.
 * @param taille_tournee Nombre de villes dans la tournée.
 */
void affichage_test_python(FILE * output_file,char * filename, char * method, double sec, double length, int * tournee, int taille_tournee){
    
    fprintf(output_file,"%s %s %.6f %.2f [", filename, method, sec, length);

    for(int i = 0; i < taille_tournee-1; i++){
        fprintf(output_file,"%d,",tournee[i]);
    }
    fprintf(output_file,"%d]\n",tournee[taille_tournee-1]);
}