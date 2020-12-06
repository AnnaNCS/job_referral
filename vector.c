#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void av_init(struct address_vector* av_p){

  av_p->size = 0;
  av_p->buffer_size = 5;
  av_p->buffer_p = (void**) malloc(sizeof(void*) * av_p->buffer_size);
  
  return;
}

struct address_vector* av_create(){

  struct address_vector* av_p = malloc(sizeof(struct address_vector));
  av_init(av_p);
  return av_p;
} 

void av_delete(struct address_vector* av_p){

  free(av_p->buffer_p);
  free(av_p);
}

struct address_vector* av_copy(struct address_vector* to_copy){

  struct address_vector* new_av =  av_create();
  // bug, the to_copy might be greater than the av_create, cannot use memcpy
  // memcpy(new_av->buffer_p, to_copy->buffer_p, to_copy->)
  for(int i = 0; i < to_copy->size; i++){
    av_append(new_av, to_copy->buffer_p[i]);
  }
  return new_av;
}

void av_pop(struct address_vector* av_p){

  void** buffer_new = (void**) malloc(sizeof(void*) * av_p->buffer_size);
  memcpy(buffer_new, av_p->buffer_p + 1, (av_p->buffer_size -1) * sizeof(void*));
  free(av_p->buffer_p);
  av_p->buffer_p = buffer_new;
  av_p->size--;
} 

void av_append(struct address_vector* av_p, void* address){

  if(av_p->size == av_p->buffer_size){
    // STEP 1: 
    // we want to create a new buffer twice as big
    void** buffer_new = (void**) malloc(sizeof(void*) * 2 * av_p->buffer_size);
    // STEP 2:
    // call memcpy() to copy the memory, from the old one to the new one, delete previous old buffer 
    memcpy(buffer_new, av_p->buffer_p, av_p->buffer_size * sizeof(void*));
    free(av_p->buffer_p);
    av_p->buffer_p = buffer_new;
    // STEP 3: 
    // reassign the buffer_size to the size of the new buffer 
    av_p->buffer_size = 2 * av_p->buffer_size;
  }

  av_p->buffer_p[av_p->size] = address;
  av_p->size++;
}

int string_comparator(void* a, void* b){

  return !strcmp((char*) a, (char*) b);
}

void* av_search(struct address_vector* av_p, void* target_p, int (*comp)(void*,void*)){

  for (int i = 0; i < av_p->size; i++){
    
    if ((*comp)(av_p->buffer_p[i], target_p)) 
      // If value to which av_p->buffer_p[i] points to "equals" to value target
      return av_p->buffer_p[i];

  }
  return NULL;
}

void* av_back(struct address_vector* av_p){
  return av_p->buffer_p[av_p->size-1];
}