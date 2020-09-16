#include "vector.h"
#include <stdio.h>
#include <assert.h>

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
  char* anton = "anton"; // "anton" is a string literal
  // char* anna = "anna";
  // char* sasha = "sasha";
  // char* misha = "misha";
  // char* alex = "alex";
  int old_bufize = test_vector.buffer_size;
  for(int i = 0 ; i < test_vector.buffer_size + 1; i++){
    av_append(anton);
  }
  assert(old_bufize < test_vector.buffer_size && "buffer siz")

  // av_append(&my_av, anton);
  // av_append(&my_av, anna);
  // av_append(&my_av, sasha);
  // av_append(&my_av, misha);
  // av_append(&my_av, alex);


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