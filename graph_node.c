#include "graph_node.h"
#include "vector.h"
#include <string.h>
#include <stdlib.h>

int gn_comparator(void* a, void* b){

    struct graph_node* gn_1_temp = a;
    struct graph_node* gn_2_temp = b;
    if(strcmp(gn_1_temp->node_id, gn_2_temp->node_id)){
        return 0;
    }else{
        return 1;
    }
    //char* is also a string
}

void gn_init(struct graph_node* gn_p, char* node_id){

    char* node_id_temp = (char*)malloc(sizeof(char) * (strlen(node_id) + 1) ); // in order to be storred correctly you need an extra space fro the nulll terminator 
    strcpy(node_id_temp, node_id);
    gn_p->node_id = node_id_temp;

    av_init(&gn_p->neighbors); // gn_p this is a poiniter, we need its address 
}

void gn_add_neighbor(struct graph_node* gn_p, struct graph_node* neighbor){

    //first we need to check if the neighboor already is in the av neighbors 
    struct graph_node* neighbor_temp = av_search(&gn_p->neighbors, neighbor, gn_comparator);
    if (neighbor_temp == NULL){
        av_append(&gn_p->neighbors, neighbor);
    }
}