#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "struct.h"

extern tTournee best_tour;
extern tTournee current_perm;
extern double best_length;

void sigint_handler(int sig) {
    printf("\n=== Interruption (Ctrl+C) détectée ===\n");
    printf("Longueur actuelle : %.2lf\n", best_length);
    printf("(Pas d’affichage de tournée dans cette version)\n");
    exit(0);
}

void setup_interrupt_handler(void) {
    signal(SIGINT, sigint_handler);
}








