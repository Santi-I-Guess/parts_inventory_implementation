#ifndef ALLOCATOR_H
#define ALLOCATOR_H 1

#define BUFFER_SIZE 32

#include "node.h"

// using a linked list of buckets realloc never needs to be called,
// and thus all node addresses given out are stable over the lifetime
// of the program
typedef struct BUCKET {
    int count;
    Node buffer[BUFFER_SIZE];
    struct BUCKET *next_bucket;
} Bucket;

// NodeAllocator is really just a wrapper to a linked list of buckets,
// but for the sake of clarity, I'm using the struct anyways
typedef struct {
    Bucket *top_bucket;
} NodeAllocator;

void init_allocator(NodeAllocator *allocator);
void free_allocator(NodeAllocator *allocator);
Node *alloc_new_node(NodeAllocator *allocator);

#endif
