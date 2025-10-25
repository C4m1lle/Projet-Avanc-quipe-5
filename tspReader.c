#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

FILE* pfile;

tProbleme create_problem() {
    tProbleme problem = malloc(sizeof(struct s_probleme));
    if (!problem) {
        fprintf(stderr, "Erreur allocation tProbleme\n");
        return NULL;
    }
    problem->name = NULL;
    problem->comment = NULL;
    problem->type = NULL;
    problem->edge_weight_type = NULL;
    problem->display_data_type = NULL;
    problem->dimension = 0;
    problem->tournee = NULL;
    return problem;
}

tProbleme load_problem(const char *filepath) {
    char buffer[128];
    int id;
    double x, y;
    tInstance current_instance;

    if ((pfile = fopen(filepath,"r")) == NULL) {
        fprintf(stderr,"Fichier %s inaccessible.\n", filepath);
        return NULL;
    }

    tProbleme problem = create_problem();

    while (fscanf(pfile, "%s", buffer) != EOF) {
        if (strcmp(buffer, "NAME:") == 0) fscanf(pfile, "%s", buffer), problem->name = strdup(buffer);
        else if (strcmp(buffer, "COMMENT:") == 0) fscanf(pfile, "%[^\n]", buffer), problem->comment = strdup(buffer);
        else if (strcmp(buffer, "TYPE:") == 0) fscanf(pfile, "%s", buffer), problem->type = strdup(buffer);
        else if (strcmp(buffer, "DIMENSION:") == 0) fscanf(pfile, "%d", &problem->dimension);
        else if (strcmp(buffer, "EDGE_WEIGHT_TYPE:") == 0) fscanf(pfile, "%s", buffer), problem->edge_weight_type = strdup(buffer);
        else if (strcmp(buffer, "NODE_COORD_SECTION") == 0) {
            problem->tournee = create_tournee(problem->dimension);
            for (int i = 0; i < problem->dimension; i++) {
                fscanf(pfile, "%d %lf %lf", &id, &x, &y);
                current_instance = create_instance(id, x, y);
                add_in_tournee(problem->tournee, current_instance);
            }
        }
        else if (strcmp(buffer, "DISPLAY_DATA_TYPE:") == 0) fscanf(pfile, "%s", buffer), problem->display_data_type = strdup(buffer);
    }

    fclose(pfile);
    return problem;
}

void print_nodes(tProbleme problem) {
    tInstance inst;
    printf("[");
    for (int i = 0; i < problem->dimension; i++) {
        inst = get_instance_at(problem->tournee, i);
        printf("%d%s", get_id(inst), (i == problem->dimension - 1) ? "]\n" : ", ");
    }
}

void print_values(tProbleme problem) {
    tInstance inst;
    printf("[");
    for (int i = 0; i < problem->dimension; i++) {
        inst = get_instance_at(problem->tournee, i);
        printf("(%.2lf, %.2lf)%s", get_x(inst), get_y(inst), (i == problem->dimension - 1) ? "]\n" : ", ");
    }
}

void delete_problem(tProbleme *pproblem) {
    if (!pproblem || !*pproblem) return;
    tProbleme p = *pproblem;
    free(p->name);
    free(p->comment);
    free(p->type);
    free(p->edge_weight_type);
    free(p->display_data_type);
    if (p->tournee) delete_tournee(&p->tournee);
    free(p);
    *pproblem = NULL;
}



