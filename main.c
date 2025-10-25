#include <stdio.h>
#include <stdlib.h>
#include "struct.h"
#include "tspReader.h"
#include "interrupt_handler.h"

tTournee best_tour = NULL;
tTournee current_perm = NULL;
double best_length = 0.0;

int main(void) {
    tProbleme problem = load_problem("a280.tsp");
    if (!problem) return 1;

    setup_interrupt_handler();

    best_tour = create_tournee(problem->dimension);
    current_perm = create_tournee(problem->dimension);

    printf("TSP chargé : %s\n", problem->name);
    print_nodes(problem);
    print_values(problem);

    delete_problem(&problem);
    delete_tournee(&best_tour);
    delete_tournee(&current_perm);

    return 0;
}


































