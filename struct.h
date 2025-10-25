#ifndef STRUCT_H
#define STRUCT_H

typedef struct s_instance* tInstance;
typedef struct s_tournee* tTournee;
typedef struct s_probleme* tProbleme;

struct s_instance {
    int id;
    double x;
    double y;
};

struct s_tournee {
    tInstance *chemin;
    int taille;
};

struct s_probleme {
    char *name;
    char *comment;
    char *type;
    int dimension;
    char *edge_weight_type;
    char *display_data_type;
    tTournee tournee;
};

/* Déclarations des fonctions */
tInstance create_instance(int id, double x, double y);
tTournee create_tournee(int taille);
void add_in_tournee(tTournee tour, tInstance inst);
tInstance get_instance_at(tTournee tour, int index);
void delete_tournee(tTournee *tour);

#endif







