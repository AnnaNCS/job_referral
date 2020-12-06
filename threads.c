#include "box.h"
#include "threads.h"
#include <stdio.h>
#include "graph_node.h"
#include <unistd.h>
#include "vector.h"

void* search_thread_main(void* t_args){
    struct SearchThreadArgs* args = (struct SearchThreadArgs*) t_args;
    struct Box* in_box = args->in_box;
    struct Box* out_box = args->out_box;
    struct graph_node* graph = args->graph;
    int graph_size = args->graph_size;

    while(!(*args->quit_pressed)){
        
        // Step 1: Take the input from input thread
        // 1.1 Take the lock
        
        pthread_mutex_lock(&in_box->lock);
    
        if (in_box->av.size == 0) {
            pthread_mutex_unlock(&in_box->lock);
            sleep(1);
            continue;
        }
        // 1.2 Take input from the box or if empty, remember
        struct pair* input_pair = av_back(&in_box->av);
        av_pop(&in_box->av);

        // 1.3 Release the lock
        pthread_mutex_unlock(&in_box->lock);
        // 1.4 If no input, sleep a bit and "continue"
        
        // Step 2: Call search function
        struct address_vector* final_path = find_path(input_pair, graph, graph_size);

        // Step 3: Pass output from search function to output thread
        
        // 3.1 Take the lock
        pthread_mutex_lock(&out_box->lock);
        // 3.2 Push back the input
        av_append(&out_box->av,final_path);
        // 3.3 Release the lock
        pthread_mutex_unlock(&out_box->lock);

        delete_pair(input_pair);
    }

    fprintf(stderr, "Search thread quitting\n");
    return NULL;
}


void* output_thread_main(void* t_args){
    struct OutputThreadArgs* args = (struct OutputThreadArgs*) t_args;
    struct Box* out_box = args->out_box;
    char* file_name = args->file_name;
    bool* quit_pressed = args->quit_pressed;

    FILE *fptr;
    fptr = fopen(file_name, "w+");
    // Step 1: Open file

    while(!(*args->quit_pressed)){

        // Step 2: Get input from box
        
        // 2.1 Grab the lock
        pthread_mutex_lock(&out_box->lock);

        if (out_box->av.size == 0) {
            pthread_mutex_unlock(&out_box->lock);
            sleep(1);
            continue;
        }
        struct address_vector* output_path = av_back(&out_box->av);
        // 2.2 Pop from the box
        av_pop(&out_box->av);
        // 2.3 Release lock
        pthread_mutex_unlock(&out_box->lock);
        // 2.4 Sleep if no input, and continue
        
        // Step 3: Write to file
        if(output_path == NULL){
            printf("The pathway does not exist between x and x, try again:\n");
            continue;
        }else{
            for (int i = 0; i < output_path->size; i++){
                struct graph_node* node = output_path->buffer_p[i];
                char* name = node->node_id;
                printf(" -> %s", name);
                fprintf(fptr, " -> %s", name);
            }
            printf("\n");
            fprintf(fptr, "\n");
            av_delete(output_path);
        }
    }
    // Step 4: IMPORTANT CLOSE THE FILE
    fclose(fptr);
    fprintf(stderr, "Output thread quitting\n");
    return NULL;
}