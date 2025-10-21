#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "struct.h"
#include "tspReader.h"

enum WTYPE = {EUCL_2D, GEO, ATT};


struct s_probleme {
    

    char * COMMENT;
    int DIMENSION;
    enum WTYPE EDGE_WEIGHT_TYPE;
    NODE COORD TYPE


    NODE_COORD_SECTION
    EDGE_DATA_SECTION
    tTournee tournee;
}



tProbleme load_problem(const char * filepath){
    
    if((fd = open(filepath,O_RDONLY)) == -1){
        fprintf(stderr,"Fichier %s inaccessible ou inexistant.\n",filepath);
        return NULL;
    }
    

}