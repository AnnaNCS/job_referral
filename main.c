#include "vector.h"
#include "graph_node.h"
#include "pair.h"
#include "box.h"
#include "threads.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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

  struct Box* in_box = create_box();
  struct Box* out_box = create_box();
  bool quit_pressed = false;
  

  struct SearchThreadArgs args;
  args.graph = graph;
  args.graph_size = unique_names.size;
  args.in_box = in_box;
  args.out_box = out_box;
  args.quit_pressed = &quit_pressed;

  struct OutputThreadArgs out_args;
  out_args.file_name = "output.txt";
  out_args.out_box = out_box;
  out_args.quit_pressed = &quit_pressed;

  pthread_t search_thread;
  pthread_t output_thread;
  pthread_create(&search_thread, NULL, search_thread_main, (void*) &args);
  pthread_create(&output_thread, NULL, output_thread_main, (void*) &out_args);

  char* line = NULL;
  char* input_1 = NULL;
  char* input_2 = NULL;
  const char* quit = "quit";
    
  fprintf(stdout, "Input two names:\n");
  size_t n = 0;
  const char* delimmeter = ",";

  int read;
  
  while(1){

    read = getline(&line, &n, stdin);

    line[read-1] = 0;
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
      while(1){
        if((in_box->av.size == 0) && (out_box->av.size == 0)){
          break;
        }
        sleep(3);
      }
      quit_pressed = true;
      pthread_join(search_thread, NULL);
      pthread_join(output_thread, NULL);
      fprintf(stderr, "Thank you, the program will terminate now.\n");
      return;
    }

    if(input_2 == NULL){
      fprintf(stdout, "Your input was not complete, try again:\n");
      continue;
    }

    fprintf(stdout, "Your input names were: %s and %s\n", input_1, input_2);

    struct pair* in_pair = create_pair(input_1, input_2);

    pthread_mutex_lock(&in_box->lock);
    av_append(&in_box->av, in_pair);
    pthread_mutex_unlock(&in_box->lock);
    
  }
    // if final path is null and try delete = crash
    
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
