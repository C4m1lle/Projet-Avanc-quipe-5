#ifndef __STRUCT_H__
#define __STRUCT_H__	

typedef struct s_instance* tInstance;
typedef struct s_tournee* tTournee;


tInstance create_instance(int idd, double xd, double yd);
void delete_instance(tInstance * pinst);
tTournee create_tournee(int tailledef);
int add_in_tournee(tTournee tour, tInstance inst);
void delete_tournee(tTournee * ptour);

double get_x(tInstance inst);
double get_y(tInstance inst);
int get_id(tInstance inst);

tInstance get_instance_at(tTournee tour,int nb);
int get_taille_tournee(tTournee tour);
int reverse_segment(tTournee tournee, int id1, int id2);

void delete_tournee_without_instances(tTournee *ptour);
void set_instance_at(tTournee tour, int index, tInstance inst);
tInstance * get_chemin_tournee(tTournee tour);


#endif
