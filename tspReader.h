#ifndef TSPREADER_H
#define TSPREADER_H

#include "struct.h"

struct s_probleme {
    char * name;
    char * comment;
    char * type;
    int dimension;
    char * edge_weight_type;
    char * display_data_type;
    tTournee tournee;
};

typedef struct s_probleme* tProbleme;

// prototypes
tProbleme create_problem(void);
tProbleme load_problem(const char * filepath);
void print_nodes(tProbleme problem);
void print_values(tProbleme problem);
tTournee get_nodes(tProbleme problem);
int get_tour_size(tProbleme problem);
const char *get_edge_weight_type(tProbleme problem);
void delete_problem(tProbleme *pproblem);

#endif


