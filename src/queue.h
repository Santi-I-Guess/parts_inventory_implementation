#ifndef QUEUE_H
#define QUEUE_H 1

#include "node.h"

typedef struct {
    int head_idx;
    int tail_idx;
    int count;
    int capacity;
    Node **data;
} Queue;

// basic funcs
void init_queue(Queue *queue);
void free_queue(Queue *queue);
void double_cap_queue(Queue *queue);

#ifdef DEBUG
void print_queue(const Queue *queue);
#endif

// write funcs
void enqueue(Queue *queue, Node *value);
Node *dequeue(Queue *queue);

#endif
