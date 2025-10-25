#ifndef __INTERRUPT_HANDLER_H__
#define __INTERRUPT_HANDLER_H__

#include "struct.h"

// Variables globales accessibles au handler

extern tTournee best_tour;
extern double best_length;
extern tTournee current_perm;

// Fonction d'installation du handler
void setup_interrupt_handler(void);

#endif
