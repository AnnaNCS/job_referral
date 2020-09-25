#include "vector.h"
#include "graph_node.h"
#include "pair.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

void find_path(char * name_1, char * name_2, struct graph_node* graph, int size){


}


struct graph_node* find_node_by_name(struct graph_node* g, char* name, int size){
    for(int i = 0; i < size; i++){
      if(strcmp(g[i].node_id, name) != 0){
        return &g[i];
      } 
    }
    return NULL;
}


int int_comparator(void* a, void* b){
  int first = *((int*) a); // gives me the value pointed to by a, &gives me the address of the value pointed by a
  int second = *((int*) b);
  return (first == second); 
}

void test_gn_init(){
  //TODO: write test 
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
// do the same thing with an int_comparator


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

// the point of this function is to makes sure that our function does what it is supposed to do

int main(int argc, char** argv) {
  
  char* program_name = argv[0];

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

  // for each unique value we create a node // 
  struct graph_node* graph = malloc(sizeof(struct graph_node) * unique_names.size);
  for(int i = 0; i < unique_names.size; i++){
    char* name = unique_names.buffer_p[i];
    gn_init(&graph[i], name);
  }
  // you use a -> with a pointer
  // we then go back to our vector with each pair, take both names, and append them to their neighbors//
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

  // we read user input, and call BFS search to find the path from one person to another
  //getline() 
  
  /*
  struct address_vector my_av;

  av_init(&my_av);

  printf("Testing av_init ... ");
  test_av_init();
  printf("PASS\n");

  printf("Testing av_append ... "); 
  test_av_append();
  printf("PASS\n");

  struct graph_node gn_temp;
  gn_init(&gn_temp, "alex");
  */
 

  return 0;
}