#ifndef __THREADS__H__
#define __THREADS__H__
#include "box.h"
#include <stdbool.h> 

struct SearchThreadArgs {
    struct Box* in_box;
    struct Box* out_box;
    bool* quit_pressed;
    struct graph_node* graph;
    int graph_size; 
};

struct OutputThreadArgs {
    char* file_name;
    struct Box* out_box;
    bool* quit_pressed;
   
};

void* search_thread_main(void* t_args);
void* output_thread_main(void* t_args);

#endif