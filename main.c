#include "vector.h"
#include "graph_node.h"
#include "pair.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

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

struct address_vector* find_path(char* name_1, char* name_2, struct graph_node* graph, int size){

  //fprintf(stderr, "nperforming path search %d\n", size);

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

int int_comparator(void* a, void* b){

  int first = *((int*) a); // gives me the value pointed to by a, &gives me the address of the value pointed by a
  int second = *((int*) b);
  return (first == second); 
}

void append_neighbors_to_names(struct address_vector input_pairs, struct address_vector unique_names, struct graph_node* graph){

  for(int i = 0; i < input_pairs.size; i++){
    struct pair* curr_pair = input_pairs.buffer_p[i];
    char* name_1 = curr_pair->name_1;
    char* name_2 = curr_pair->name_2;
    
    struct graph_node* n1 = find_node_by_name(graph, name_1, unique_names.size);
    struct graph_node* n2 = find_node_by_name(graph, name_2, unique_names.size);
    // find the node of my name, looking for it in my graph |^
    gn_add_neighbor(n1, n2);
    gn_add_neighbor(n2, n1);
  }

}

void read_user_input(struct graph_node* graph, struct address_vector unique_names){
  

  char* line = NULL;
  char* input_1 = NULL;
  char* input_2 = NULL;
  char* quit = "quit";
    
  fprintf(stdout, "Input two names:\n");
  size_t n = 0;
  const char* delimmeter = ",";

  int read;
  
  while(1){
    
    fprintf(stdout, ">");

    read = getline(&line, &n, stdin);

    line[read-1] = 0; // NOTE: must remove new line character
    input_1 = strtok(line, delimmeter);
    input_2 = strtok(NULL, delimmeter);

    

    if(input_1 == NULL){
      fprintf(stdout, "Your input was not complete, try again:\n");
      continue;
    }

    if(input_1 == input_2){
      fprintf(stdout, "Your input names are the same, try again:\n");
      continue;
    }

    if(strcmp(input_1, quit) == 0){
      fprintf(stdout, "Thank you, the program will terminate now.\n");
      return;
    }

    if(input_2 == NULL){
      fprintf(stdout, "Your input was not complete, try again:\n");
      continue;
    }

    fprintf(stdout, "Your input names were: %s and %s\n", input_1, input_2);
      
    struct address_vector* final_path = find_path(input_1, input_2, graph, unique_names.size);

    // you are unable to take a size of a null pointer, you can not dereference it
    
    if(final_path == NULL){
      printf("The pathway does not exist, try again:\n");
      continue;
    }else{
      for (int i = 0; i < final_path->size; i++){
        struct graph_node* node = final_path->buffer_p[i];
        char* name = node->node_id;
        printf(" -> %s", name);
      }
      printf("\n");
      
    }
    av_delete(final_path); // if final path is null and try delete = crash
  }
}


void test_av_init(){

  struct address_vector test_vector;
  test_vector.buffer_p = NULL;
  //av_append(&test_vector, NULL); can not append here, as we have not initialized the av. Even tho it exists,
  // it is not usable yet, so you can not do anything with it (broken)
  av_init(&test_vector);

  assert(test_vector.size == 0 && "new vector size is 0");
  // an invalid buffer size ex: -3 or 0
  assert(test_vector.buffer_size > 0 && "new vector buffer size is positive");
  assert(test_vector.buffer_p != NULL && "new vector has its memory buffer allocated");
}

void test_av_search(){
  // use int_comparator and string_comparator 
  // we want to cheack that the search return us the correct item 

  struct address_vector test_vector;
  av_init(&test_vector);
  char* anna_p = "anna";
  av_append(&test_vector, anna_p);
  av_search(&test_vector, anna_p, string_comparator); 
  //assert() we want to compare the return pointer from the research funciton to the pointer of our target
  //the pointers themselves 
}

void test_av_append(){

  struct address_vector test_vector;
  av_init(&test_vector);

  // testing
  char* anton_p = "anton"; // "anton" is a string literal
  
  int old_bufize = test_vector.buffer_size;
  void** old_buffer_p = test_vector.buffer_p;
  for(int i = 0 ; i < old_bufize + 1; i++){
    av_append(&test_vector, anton_p); // infinite loop (replace buffer.zise with old_bufsize)
    assert(anton_p == test_vector.buffer_p[i] && "the element is not in the address vector");
    assert(test_vector.size == (i + 1) && "buffer size not updated");
  }
  assert(old_bufize < test_vector.buffer_size && "buffer size not increased after resizing");
  assert(old_buffer_p != test_vector.buffer_p && "buffer address not changed");

  //we also want to make sure we have correctly reallocated our buffer_p 
  //by checking the adress of the old and new one, so that they do not much
}

int main(int argc, char** argv) {
  
  struct address_vector input_pairs;
  av_init(&input_pairs);

  FILE* fp = fopen("file.txt", "r");

  // we read a file// 
  file_to_vector_pairs(fp, &input_pairs);

  fclose(fp);
  
  struct address_vector unique_names;
  av_init(&unique_names);
  // we then want to go through each pair and collect unique values //
  find_unique_names(&input_pairs, &unique_names);
  
  // make a change here, so that our graph is a address vector 
  // for each unique value we create a node // 
  struct graph_node* graph = malloc(sizeof(struct graph_node) * unique_names.size);
  for(int i = 0; i < unique_names.size; i++){
    char* name = unique_names.buffer_p[i];
    //printf("unique name %s\n", name);
    gn_init(&graph[i], name);
  }

  // you use a -> with a pointer
  // we then go back to our vector with each pair, take both names, and append them to their neighbors//
  append_neighbors_to_names(input_pairs, unique_names, graph);

  // we read user input, and call BFS search to find the path from one person to another
  read_user_input(graph, unique_names);

  return 0;
}