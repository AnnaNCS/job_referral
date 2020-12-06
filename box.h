#ifndef __BOX__H__
#define __BOX__H__
#include "vector.h"
#include <pthread.h>


struct Box {
    pthread_mutex_t lock;
    struct address_vector av;
};

struct Box* create_box();
void box_init (struct Box* box);

#endif