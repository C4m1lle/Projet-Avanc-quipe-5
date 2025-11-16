#ifndef __TSPREADER_H__
#define __TSPREADER_H__	

typedef struct s_probleme* tProbleme;

tProbleme load_problem(const char * filepath);
void print_nodes(tProbleme problem);
tTournee get_nodes(tProbleme problem);
void print_values(tProbleme problem);
const char *get_edge_weight_type(tProbleme problem);
void delete_problem(tProbleme *pproblem);
int get_size_probleme(tProbleme prob);
#endif
