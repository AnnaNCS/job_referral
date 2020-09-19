#include "vector.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>


int string_comparator(void* a, void*b){

  return !strcmp(a, b);
}

int int_comparator(void* a, void* b){
  int first = *((int*) a);
  int second = *((int*) b);
  return (a == b); 
}

void test_av_init(){

  struct address_vector test_vector;
  test_vector.buffer_p = NULL;
  av_init(&test_vector);

  assert(test_vector.size == 0 && "new vector size is 0");
  // an invalid buffer size ex: -3 or 0
  assert(test_vector.buffer_size > 0 && "new vector buffer size is positive");
  assert(test_vector.buffer_p != NULL && "new vector has its memory buffer allocated");
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

// the point of this function is to makes sure that our function does what it is supposed to do

int main(int argc, char** argv) {

  char* program_name = argv[0];

  struct address_vector my_av;

  // Call the init function to allocated memory, set the size and other arguments
  av_init(&my_av);

  printf("Testing av_init ... ");
  test_av_init();
  printf("PASS\n");

  printf("Testing av_append ... "); 
  test_av_append();
  printf("PASS\n");

  return 0;
}