#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef DEBUG
#include <stdio.h>
#endif

#include "queue.h"

// basic funcs
void init_queue(Queue *queue) {
    queue->data = calloc(8, sizeof(Node*));
    queue->head_idx = 0;
    queue->tail_idx = 0;
    queue->count = 0;
    queue->capacity = 8;
}

void free_queue(Queue *queue) {
    if (queue->data != NULL)
        free(queue->data);
    memset(queue, 0, sizeof(Queue));
}

void double_cap_queue(Queue *queue) {
    int new_size = sizeof(Queue) * (queue->capacity << 1);
    Node **tmp_data = realloc(queue->data, new_size);

    int old_size = sizeof(Queue) * (queue->capacity);
    memset(&tmp_data[queue->capacity], 0, old_size);
    queue->capacity <<= 1;
    queue->data = tmp_data;

    // move disconnected "behind" elements in front of head
    if (queue->head_idx >= queue->tail_idx) {
        int offset = queue->head_idx + queue->count - queue->tail_idx;
        for (int i = 0; i < queue->tail_idx; ++i) {
            queue->data[offset+i] = queue->data[i];
            queue->data[i] = NULL;
        }
        queue->tail_idx = queue->head_idx + queue->count;
    }

    // shift elements left until no empty elements remain
    for (int i = 0; i < queue->count; ++i) {
        queue->data[i] = queue->data[queue->head_idx+i];
    }
    queue->head_idx = 0;
    queue->tail_idx = queue->count;
    memset(
        &queue->data[queue->count],
        0,
        sizeof(Queue) * (queue->capacity - queue->count)
    );
}

#ifdef DEBUG
void print_queue(const Queue *queue) {
    for (int i = 0; i < queue->capacity; ++i) {
        printf("(%2d): %p", i, (void*)queue->data[i]);
        // head_idx should never equal tail_idx
        if (i == queue->head_idx && queue->head_idx == queue->tail_idx) {
            printf(" <- head & tail\n");
        } else if (i == queue->head_idx) {
            printf(" <- head\n");
        } else if (i == queue->tail_idx) {
            printf(" <- tail\n");
        } else {
            printf("\n");
        }
    }
    printf("\n");
}
#endif

// write funcs
void enqueue(Queue *queue, Node *value) {
    if (queue->count == queue->capacity - 1) {
        double_cap_queue(queue);
    }

    queue->data[queue->tail_idx] = value;
    if (queue->tail_idx == queue->capacity - 1)
        queue->tail_idx = 0;
    else
        queue->tail_idx++;
    queue->count++;
}

Node *dequeue(Queue *queue) {
    if (queue->count == 0) {
        queue->head_idx = 0;
        queue->tail_idx = 0;
        return NULL;
    }

    Node *value = queue->data[queue->head_idx];
    queue->data[queue->head_idx] = NULL;
    if (queue->head_idx == queue->capacity - 1)
        queue->head_idx = 0;
    else
        queue->head_idx++;
    queue->count--;

    return value;
}
