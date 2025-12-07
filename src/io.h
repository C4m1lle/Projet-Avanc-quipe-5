/**
 * @file io.h
 * @brief Fonctions d'entrée/sortie pour l'application TSP.
 *
 * Contient les déclarations pour afficher l'usage du programme et
 * produire des sorties formatées, notamment compatibles avec Python.
 */

#ifndef __IO_FILE_H__
#define __IO_FILE_H__

#include <stdio.h>

/**
 * @brief Affiche l'usage du programme TSP.
 *
 * Cette fonction décrit les options disponibles pour exécuter le programme,
 * y compris le fichier d'entrée, la distance choisie et la méthode de résolution.
 *
 * @param arg Nom de l'exécutable (argv[0]).
 */
void usage(char * arg);

/**
 * @brief Affiche les résultats de l'exécution au format compatible Python.
 *
 * La sortie produite contient :
 *   filename method runtime distance [tour]
 *
 * @param output_file    Fichier de sortie (stdout ou fichier .txt).
 * @param filename       Nom du fichier TSP traité.
 * @param method         Nom de la méthode utilisée (bf, nn, rw, etc.).
 * @param sec            Temps d'exécution en secondes.
 * @param length         Distance totale de la tournée calculée.
 * @param tournee        Tableau des IDs des villes dans l'ordre de la tournée.
 * @param taille_tournee Nombre de villes dans la tournée.
 */
void affichage_test_python(FILE * output_file,char * filename, char * method, double sec, double length, int * tournee, int taille_tournee);
//int param(int argc, char *argv[]);
#endif
