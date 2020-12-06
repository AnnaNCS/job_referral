#include "box.h"
#include "vector.h"
#include <pthread.h>
#include <stdlib.h>

void box_init(struct Box* box){

    pthread_mutex_init(&box->lock, NULL);
    av_init(&box->av);
  
  return;
}

struct Box* create_box(){

  struct Box* box = malloc(sizeof(struct Box));
  box_init(box);
  return box;

}