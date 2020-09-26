#include "graph_node.h"
#include "pair.h"
#include "vector.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


struct pair* create_pair(char* name_1, char* name_2){
    char* name_one = (char*) malloc(sizeof(char) * strlen(name_1) + 1);
    char* name_two = (char*) malloc(sizeof(char) * strlen(name_2) + 1);
    strcpy(name_one, name_1);
    strcpy(name_two, name_2);
    struct pair* p = (struct pair*) malloc(sizeof(struct pair));
    p->name_1 = name_one;
    p->name_2 = name_two;
    return p;
    //(*p).name_1 = name_one;
}


void file_to_vector_pairs(FILE *fp, struct address_vector* my_pairs){

    char * line = NULL;
    size_t n = 0;
    size_t read = 0;

    while ((read = getline(&line, &n, fp)) != -1){
        line[read -1] = 0;
        const char* delimmeter = ",";
        char* name_1 = NULL;
        char* name_2 = NULL;
        name_1 = strtok(line, delimmeter);
        name_2 = strtok(NULL, delimmeter);
        struct pair* p = create_pair(name_1, name_2);
        av_append(my_pairs, p);

        //*(line + (read -1))
    }

    if(line != NULL){
        free(line);
    }
     
}

void find_unique_names(struct address_vector* input_pairs, struct address_vector* unique_names){
    for(int i = 0; i < input_pairs->size; i++){
        struct pair* curr_pair = input_pairs->buffer_p[i];
        char* temp = av_search(unique_names, curr_pair->name_1, string_comparator);
        if(temp == NULL){
            av_append(unique_names, curr_pair->name_1);

        }
        if(av_search(unique_names, curr_pair->name_2, string_comparator) == NULL){
            av_append(unique_names, curr_pair->name_2);
        }
    }
    

}