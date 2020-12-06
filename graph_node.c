#include "graph_node.h"
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

struct graph_node* find_node_by_name(struct graph_node* g, char* name, int size){

    for(int i = 0; i < size; i++){
      //printf("%d\n", i);
      if(strcmp(g[i].node_id, name) == 0){
        
        return &g[i];
      } 
    }
    //printf("Not found\n");
    return NULL;
}

struct address_vector* find_path(struct pair* input_pair, struct graph_node* graph, int size){

  //fprintf(stderr, "nperforming path search %d\n", size);
  char* name_1 = input_pair->name_1;
  char* name_2 = input_pair->name_2;

  struct address_vector* final_path = NULL;

  // first we want to find the node that we are going to start from 
  struct graph_node* node_start = find_node_by_name(graph, name_1, size);

  // the same comes to the node we want to finish our search with 
  struct graph_node* node_end = find_node_by_name(graph, name_2, size);

  if(node_start == NULL || node_end == NULL){
    return final_path;
  }

  struct address_vector* visited = av_create();
  struct address_vector* queue = av_create();

  //fprintf(stderr, "node ID  start %s ; end %s\n", node_start->node_id, node_end->node_id);

  // next, we want to enqueue our first node 
  struct address_vector* path = av_create();

  av_append(path, node_start);
  av_append(queue, path);

  struct graph_node* curr_node; 
  curr_node = node_start;
  
  while(queue->size != 0){
    
    path = queue->buffer_p[0];
    av_pop(queue);

    curr_node = path->buffer_p[path->size - 1];
    //fprintf(stderr, "visiting node id %s\n", curr_node->node_id);
    if (curr_node == node_end){
      final_path = path;
      //printf("The pathway to your target is completed\n");
      break;
    }

    av_append(visited, curr_node);

    for(int i = 0; i < curr_node->neighbors.size; i++){
      struct graph_node* neighbor = curr_node->neighbors.buffer_p[i];
      if(av_search(visited, neighbor, gn_comparator) == NULL){
        struct address_vector* path_to_neighbor = av_copy(path);
        av_append(path_to_neighbor, neighbor);
        av_append(queue, path_to_neighbor);
      }
    
    }
    // to prevent leak memory, as we have created and poped it earlier 
    av_delete(path);

  }
  // clean up code before deleting the queue
  for (int i = 0; i < queue->size; i++){
    av_delete(queue->buffer_p[i]);
  }

  av_delete(queue);
  av_delete(visited);


  return final_path;
}

