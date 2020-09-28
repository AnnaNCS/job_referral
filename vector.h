#ifndef __VECTOR__H__
#define __VECTOR__H__

//ADRESS VECTOR IS OUR CONTAINER 

struct address_vector {

  int size; // how many elements in the vector are currently
  int buffer_size; // how mcuh memory there is
  void** buffer_p; // allocated memory

};


void av_init(struct address_vector* av);

struct address_vector* av_create();

void av_delete(struct address_vector* av_p);

struct address_vector* av_copy(struct address_vector* to_copy);

// Append new element to end of vector
void av_append(struct address_vector* av_p, void* address);

// Removes an element from the end of the vector
void av_pop(struct address_vector* av_p);

// optional // Get the element at index. Make sure to check for invalid index.
void* av_get_at(struct address_vector* av_p, int index);

// optional // Put an element at index. Make sure to check for invalid index.
void av_put_at(struct address_vector* av_p, int index, void* address);

int string_comparator(void* a, void*b);

// Searches the pointer to the element(target_p) in the memory 
void* av_search(struct address_vector* av_p, void* target_p, int (*comp)(void*,void*));

#endif