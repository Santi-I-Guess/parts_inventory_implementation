#include <stdlib.h>
#include <string.h>

#include "allocator.h"
#include "node.h"

void init_allocator(NodeAllocator *allocator) {
    allocator->top_bucket = calloc(1, sizeof(Bucket));
}

void free_allocator(NodeAllocator *allocator) {
    // go through bucket linked list, free each "behind" bucket
    Bucket *top_bucket = allocator->top_bucket;
    while (top_bucket != NULL) {
        Bucket *prev_bucket = top_bucket;
        top_bucket = top_bucket->next_bucket;

        // cleanup array of children pointers per node, if any
        for (int i = 0; i < prev_bucket->count; ++i) {
            Node *curr_node = &prev_bucket->buffer[i];
            if (curr_node->num_children != 0)
                free(curr_node->children);
        }
        free(prev_bucket);
    }
    allocator->top_bucket = NULL;
}

Node *alloc_new_node(NodeAllocator *allocator) {
    // allocate new bucket if top bucket is full
    if (allocator->top_bucket->count == BUFFER_SIZE) {
        Bucket *new_bucket = calloc(1, sizeof(Bucket));
        Bucket *old_top_bucket = allocator->top_bucket;

        // place new_bucket as allocator's top_bucket
        // old_top_bucket now "behind" new_bucket
        old_top_bucket->next_bucket = new_bucket;
        new_bucket->next_bucket = NULL;
        allocator->top_bucket = new_bucket;
    }

    Bucket *top_bucket = allocator->top_bucket;
    Node *new_node = &top_bucket->buffer[top_bucket->count];
    top_bucket->count++;
    return new_node;
}
