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
        // Step 4: Mystery step to keep the program from bleeding memory

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
        delete_pair(input_pair);
    }

    fprintf(stderr, "Search thread quitting");
    return NULL;
}


void* output_thread_main(void* t_args){
    // struct OutputThreadArgs* args = (struct OutputThreadArg*) t_args;
    // struct Box* out_box = args->out_box;
    // char* file_name = args->file_name;

    // FILE *fptr;
    // fptr = fopen("output.txt","w");
    // // Step 1: Open file

    // while(!(*args->quit_pressed)){

    //     // Step 2: Get input from box
    //     struct address_vector* output = out_box->av.buffer_p[0];
    //     // 2.1 Grab the lock
    //     pthread_mutex_lock(&out_box->lock);

    //     if (in_box->av.size == 0) {
    //         sleep(5); 
    //         pthread_mutex_unlock(&out_box->lock);
    //         continue;
    //     }
    //     // 2.2 Pop from the box
    //     av_pop(&out_box->av);
    //     // 2.3 Release lock
    //     pthread_mutex_unlock(&out_box->lock);
    //     // 2.4 Sleep if no input, and continue
        
    //     // Step 3: Write to file
    //     fputs(output, fptr);
    //     // Step 4: Mystery step???!!
        
    // }

    // // Step 4: IMPORTANT CLOSE THE FILE
    // fclose(fptr);
    return NULL;
}