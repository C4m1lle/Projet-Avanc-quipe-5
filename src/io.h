#ifndef __IOFILE_H__
#define __IOFILE_H__

void usage(char * arg);
void affichage_test_python(FILE * output_file,char * filename, char * method, double sec, double length, int * tournee, int taille_tournee);
int param(int argc, char *argv[]);
#endif