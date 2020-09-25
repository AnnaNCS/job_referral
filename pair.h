#ifndef __PAIR__H__
#define __PAIR__H__
#include "vector.h"
#include <stdio.h>



struct pair {
    char* name_1;
    char* name_2;
};

void file_to_vector_pairs(FILE* fp, struct address_vector* my_pairs);

struct pair* create_pair(char* name_1, char* name_2);

void find_unique_names(struct address_vector* my_pairs, struct address_vector* unique_names);

#endif