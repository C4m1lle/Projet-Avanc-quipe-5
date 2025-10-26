#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include "struct.h"
#include "demi_matrice.h"
#include "distance.h"
#include "tspReader.h"
// Setup du signal handler pour Ctrl+C
void setup_signal_handler(tTournee tour, DistanceFunc dist,int * best, double * distmin, int bff);

#endif // SIGNAL_HANDLER_H

