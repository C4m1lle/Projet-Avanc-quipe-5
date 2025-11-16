#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H


// Setup du signal handler pour Ctrl+C
void setup_signal_handler(tTournee tour, DistanceFunc dist,int * best, double * distmin, int bff);

#endif // SIGNAL_HANDLER_H

