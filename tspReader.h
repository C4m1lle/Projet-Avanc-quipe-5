#ifndef __TSPREADER_H__
#define __TSPREADER_H__	

typedef struct s_probleme* tProbleme;

tProbleme load_problem(const char * filepath);
void print_nodes(tProbleme problem);
tTournee get_nodes(tProbleme problem);
#endif