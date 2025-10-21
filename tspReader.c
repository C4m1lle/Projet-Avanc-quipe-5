#include <stdio.h>
#include <stdlib.h>
# include <string.h>

#include "struct.h"
#include "tspReader.h"

#define NAME 289 //Somme des valeurs ascii
#define COMMENT 531
#define TYPE 322
#define DIMENSION 678
#define EDGE_WEIGHT_TYPE 1245
#define NODE_COORD_SECTION 1392

enum WTYPE {EUCL_2D, GEO, ATT};

struct s_probleme {
    
    char * name;
    char * comment;
    char * type;
    int dimension;
    enum WTYPE edge_weight_type;
    /*NODE COORD TYPE


    NODE_COORD_SECTION
    EDGE_DATA_SECTION*/
    tTournee tournee;
};

tProbleme create_problem(){
    tProbleme problem = malloc(sizeof(struct s_probleme));
    if(problem == NULL){
        fprintf(stderr, "Erreur : allocation échouée pour la structure \"Probleme\"\n");
        return NULL;
    }
    return problem;
}

char * string_alloc(char * buffer){
    char * string = malloc(sizeof(char)*strlen(buffer));
    strcpy(string,buffer);
    return string;
}

tProbleme load_problem(const char * filepath){
    FILE* pfile;
    if((pfile = fopen(filepath,"r")) == NULL){
        fprintf(stderr,"Fichier %s inaccessible ou inexistant.\n",filepath);
        return NULL;
    }
    tProbleme problem = create_problem();
    char token_buffer[32];
    fscanf(pfile,"%s",token_buffer);
    printf("%s\n",token_buffer);
    while(strcmp(token_buffer,"EOF")!=0){
        int i = 0;
        int sum = 0;

        while(token_buffer[i]!='\0' && i < 32){
            sum+=token_buffer[i];
            i++;
        }
        switch(sum){ // somme ascii des keywords par soucis de performance pour éviter de faire des if enchaînés
            
            case NAME:
                fscanf(pfile,"%*[^:]: %[^\n]",token_buffer);
                problem->name = string_alloc(token_buffer);
            break;
            case COMMENT:
                fscanf(pfile,"%*[^:]: %[^\n]",token_buffer);
                problem->comment = string_alloc(token_buffer);
            break;
            case TYPE:
                fscanf(pfile,"%*[^:]: %[^\n]",token_buffer);
                problem->type = string_alloc(token_buffer);
            break;
            case DIMENSION:
                printf("dim");
                fscanf(pfile,"%*[^:]: %[^\n]",token_buffer);
                problem->dimension = atoi(token_buffer);
            break;
            case EDGE_WEIGHT_TYPE:
            break;
            case NODE_COORD_SECTION:
            break;
            default:
                //fprintf(stderr,"Erreur : Mot clé invalide dans %s, \"%s\" non traité\n",filepath,token_buffer);
            break;

        }
        fscanf(pfile,"%s",token_buffer);
        if(token_buffer[strlen(token_buffer)-1]==':'){
            token_buffer[strlen(token_buffer)-1] = '\0';
        }
        printf("%s",token_buffer);
    }
    return  problem;
}

int token_id(char * word){
    return 0;
}


int main(void){
    tProbleme prob = load_problem("a280.tsp");
    printf("%p\n",prob);
    printf("%s\n",prob->name);
    printf("%s\n",prob->comment);
    printf("%s\n",prob->type);
    printf("%d\n",prob->dimension);
}