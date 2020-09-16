#ifndef __VECTOR__H__
#define __VECTOR__H__

struct address_vector {
  int size; // how many elements in the vector are currently
  int buffer_size; // how mcuh memory there is
  void** buffer_p; // allocated memory
};


void av_init(struct address_vector* av);

// Append new element to end of vector
void av_append(struct address_vector* av_p, void* address);

// Removes an element from the end of the vector
void* av_pop(struct address_vector* av_p);

// optional // Get the element at index. Make sure to check for invalid index.
void* av_get_at(struct address_vector* av_p, int index);

// optional // Put an element at index. Make sure to check for invalid index.
void av_put_at(struct address_vector* av_p, int index, void* address);

#endif