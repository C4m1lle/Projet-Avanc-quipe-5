#ifndef __STRUCT_H__
#define __STRUCT_H__	

typedef struct s_instance* tInstance;
typedef struct s_tournee* tTournee;

tInstance create_instance(int idd, float xd, float yd);
void delete_instance(tInstance * pinst);
tTournee create_tournee(int tailledef);
int add_in_tournee(tTournee tour, tInstance inst);
void delete_tournee(tTournee * ptour);


#endif