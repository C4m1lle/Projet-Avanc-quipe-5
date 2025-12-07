/**
 * @file signal_handler.h
 * @brief Déclarations pour la gestion des interruptions (Ctrl+C) pendant le bruteforce.
 */

#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include "../tsp/types.h"
#include "../distance/distance.h"

/**
 * @brief Initialise le gestionnaire de signal pour l'algorithme de bruteforce.
 *
 * Configure le handler pour le signal SIGINT (Ctrl+C). Permet à l'utilisateur
 * d'interrompre le calcul, de visualiser la permutation actuelle et la distance,
 * et de décider s'il souhaite arrêter ou continuer le calcul.
 *
 * @param tour Tournée initiale.
 * @param dist Fonction de calcul de distance.
 * @param best Tableau stockant la meilleure permutation.
 * @param distmin Pointeur vers la distance minimale trouvée.
 * @param bff Booléen indiquant le choix de la version du bruteforce (0 = classique, 1 = demi-matrice).
 */
void setup_signal_handler(tTournee tour, DistanceFunc dist,int * best, double * distmin, int bff);

#endif // SIGNAL_HANDLER_H
