#ifndef __GRAPH_NODE__H__
#define __GRAPH_NODE__H__
#include "vector.h"

struct graph_node {

    char* node_id;
    struct address_vector neighbors;

};

void gn_init(struct graph_node* gn_p, char* node_id);

void gn_add_neighbor(struct graph_node* gn_p, struct graph_node* neighbor_p);

int gn_comparator(void* gn_1_p, void* gn_2_p);

#endif