#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "tspReader.h"

#define NAME 289 //Somme des valeurs ascii
#define COMMENT 531
#define TYPE 322
#define DIMENSION 678
#define EDGE_WEIGHT_TYPE 1245
#define NODE_COORD_SECTION 1392


struct s_probleme {
    
    char * name;
    char * comment;
    char * type;
    int dimension;
    char * edge_weight_type;
    /*NODE COORD TYPE
    EDGE_DATA_SECTION*/
    tTournee tournee;
};

tProbleme create_problem(){
    tProbleme problem = malloc(sizeof(struct s_probleme));
    if(problem == NULL){
        fprintf(stderr, "Erreur : allocation échouée pour la structure \"Probleme\"\n");
        return NULL;
    }
    problem->tournee = create_tournee(280);
    if(problem->tournee == NULL){
        fprintf(stderr, "Erreur : allocation échouée pour la structure \"Probleme->Tournee\"\n");
        return NULL;
    }
    return problem;
}

char * string_alloc(char * buffer){
    char * string = malloc(sizeof(char)*strlen(buffer)+1);
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
    char token_trashbin[32];
    tInstance current_instance;
    fscanf(pfile,"%s",token_buffer);
    while(strcmp(token_buffer,"EOF")!=0){
        int i = 0;
        int sum = 0;
        int id,x,y;
        if(token_buffer[strlen(token_buffer)-1]==':'){
            token_buffer[strlen(token_buffer)-1] = '\0';
        }else if(sum!=NODE_COORD_SECTION){
            fscanf(pfile,"%s",token_trashbin); //retrait des :
            
        }
        while(token_buffer[i]!='\0' && i < 32){
            sum+=token_buffer[i];
            i++;
        }
        switch(sum){ // somme ascii des keywords par soucis de performance pour éviter de faire des if enchaînés
            
            case NAME:
                fscanf(pfile,"%s",token_buffer);
                problem->name = string_alloc(token_buffer);
            break;
            case COMMENT:
                fscanf(pfile,"%[^\n]",token_buffer);
                problem->comment = string_alloc(token_buffer);
            break;
            case TYPE:
                fscanf(pfile,"%s",token_buffer);
                problem->type = string_alloc(token_buffer);
            break;
            case DIMENSION:
                fscanf(pfile,"%s",token_buffer);
                problem->dimension = atoi(token_buffer);
            break;
            case EDGE_WEIGHT_TYPE:
                fscanf(pfile,"%s",token_buffer);
                problem->edge_weight_type = string_alloc(token_buffer);
            break;
            case NODE_COORD_SECTION:
                for(int i = 0; i<problem->dimension;i++){
                    fscanf(pfile,"%3d%3d%3d",&id,&x,&y);
                    current_instance = create_instance(id,x,y);
                    add_in_tournee(problem->tournee,current_instance);
                }
            break;
            default:
                fprintf(stderr,"Erreur : Mot clé invalide dans %s, \"%s\" non traité\n",filepath,token_buffer);
            break;

        }
        fscanf(pfile,"%s",token_buffer);
        
    }
    printf("%p\n",problem);
    printf("%s\n",problem->name);
    printf("%s\n",problem->comment);
    printf("%s\n",problem->type);
    printf("%d\n",problem->dimension);
    printf("%s\n",problem->edge_weight_type);
    return  problem;
}

void print_nodes(tProbleme problem){
    tInstance inst;
    printf("[");
    for(int i = 0;i<problem->dimension;i++){
        inst = get_instance_at(problem->tournee,i);
        if(i==problem->dimension-1){
            printf("%d]\n",get_id(inst));
        }else{
            printf("%d, ",get_id(inst));
        }
        
    }
}