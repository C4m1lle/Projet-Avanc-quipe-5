#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "struct.h"
#include "tspReader.h"

#define NAME 289 //Somme des valeurs ascii des caractères composant le mot-clé
#define COMMENT 531
#define TYPE 322
#define DIMENSION 678
#define EDGE_WEIGHT_TYPE 1245
#define NODE_COORD_SECTION 1392
#define DISPLAY_DATA_TYPE 1328

FILE* pfile;
struct s_probleme {
    
    char * name;
    char * comment;
    char * type;
    int dimension;
    char * edge_weight_type;
    char * display_data_type;
    tTournee tournee;
};

tProbleme create_problem() {
    tProbleme problem = malloc(sizeof(struct s_probleme));
    if (!problem) {
        fprintf(stderr, "Erreur : allocation échouée pour tProbleme\n");
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

char * string_alloc(char * buffer){
    char * string = malloc(sizeof(char)*strlen(buffer)+1);
    strcpy(string,buffer);
    return string;
}

tProbleme load_problem(const char * filepath){
   
    char token_buffer[128];
    char c;
    tInstance current_instance;
    int i,j;
    int sum;
    int id;
    double x,y;

    /*Ouverture du fichier*/
    if((pfile = fopen(filepath,"r")) == NULL){
        fprintf(stderr,"Fichier %s inaccessible ou inexistant.\n",filepath);
        return NULL;
    }
    tProbleme problem = create_problem();
    
    /*Lecture du fichier*/

    do{
        j=0;
        do{
            c = fgetc(pfile);
            token_buffer[j]=c;
            j++;
        }while(c != ' ' && c!= ':' && j<127 && c != '\n');
        token_buffer[j-1]='\0';
        if(c == ' '){
            while(fgetc(pfile)!=':');

        }


        /*Calcul de la somme ascii*/

        i = 0;
        sum = 0;
        while(token_buffer[i]!='\0' && i < 128){
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
                problem->tournee = create_tournee(problem->dimension);
                for(int i = 0; i<problem->dimension;i++){
                    fscanf(pfile,"%d %lf %lf",&id,&x,&y);
                    current_instance = create_instance(id,x,y);
                    add_in_tournee(problem->tournee,current_instance);
                }
            break;
            case DISPLAY_DATA_TYPE:
                fscanf(pfile,"%s",token_buffer);
                problem->display_data_type = string_alloc(token_buffer);
            break;
            case 0: //mot vide
            break;
            case 218: // EOF 
            break;
            default:
                fprintf(stderr,"Erreur : Mot clé invalide dans %s, \"%s\" non traité\n",filepath,token_buffer);
            break;

        }

    }while(strcmp(token_buffer,"EOF")!=0);
    /*printf("%p\n",problem);
    printf("%s\n",problem->name);
    printf("%s\n",problem->comment);
    printf("%s\n",problem->type);
    printf("%d\n",problem->dimension);
    printf("%s\n",problem->edge_weight_type);*/
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
void print_values(tProbleme problem){
    tInstance inst;
    printf("[");
    for(int i = 0;i<problem->dimension;i++){
        inst = get_instance_at(problem->tournee,i);
        if(i==problem->dimension-1){
            printf("(%.2lf , %.2lf)]\n",get_x(inst),get_y(inst));
        }else{
            printf("(%.2lf , %.2lf), ",get_x(inst),get_y(inst));
        }
        
    }
}

tTournee get_nodes(tProbleme problem){
    return problem->tournee;
}
const char *get_edge_weight_type(tProbleme problem) {
    if (!problem || !problem->edge_weight_type)
        return "EUC_2D"; // valeur par défaut

    // On copie la valeur du fichier TSPLIB
    const char *type = problem->edge_weight_type;

    // Normalisation : on supprime les majuscules/minuscules éventuelles
    if (strcasecmp(type, "EUC_2D") == 0)
        return "EUC_2D";
    else if (strcasecmp(type, "ATT") == 0)
        return "ATT";
    else if (strcasecmp(type, "GEO") == 0)
        return "GEO";
    else
        return "EUC_2D"; // par défaut si type inconnu
}

void delete_problem(tProbleme *pproblem) {
    if (!pproblem || !*pproblem) return;

    tProbleme p = *pproblem;

    if (p->name) free(p->name);
    if (p->comment) free(p->comment);
    if (p->type) free(p->type);
    if (p->edge_weight_type) free(p->edge_weight_type);
    if (p->display_data_type) free(p->display_data_type);

    if (p->tournee)
        delete_tournee(&(p->tournee));

    free(p);
    *pproblem = NULL;
    fclose(pfile);
}
