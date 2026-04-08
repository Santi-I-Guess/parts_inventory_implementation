#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "allocator.h"
#include "node.h"
#include "queue.h"
#include "tokenizer.h"

// wrapper function
TokenArray tokenize_input() {
    char input[80] = "";
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1] = '\0';
    remove_spaces(input);
    return tokenize_line(input);
}

// prompt for root name, enqueue root node
void setup_root_node(Node *root, Queue *queue) {
    fprintf(stderr, "what will you define?\n");
    TokenArray tokens = tokenize_input();
    strcpy(root->name, tokens.array[0]);
    root->quantity = 1;
    enqueue(queue, root);
}

int main() {
    NodeAllocator allocator;
    init_allocator(&allocator);

    Node *root = alloc_new_node(&allocator);
    Queue queue;
    init_queue(&queue);

    setup_root_node(root, &queue);
    while (queue.count > 0) {
        Node *curr_node = dequeue(&queue);
        fprintf(stderr, "what is %s?\n", curr_node->name);

        TokenArray input_tokens = tokenize_input();

#ifdef DEBUG
        for (int i = 0; i < input_tokens.num_tokens; ++i) {
            char (*curr_token)[32] = &input_tokens.array[i];
            fprintf(stderr, "DEBUG: token \"%s\"\n", *curr_token);
        }
#endif

        if (input_tokens.num_tokens == 0) {
            // no input
            fprintf(stderr, "DEBUG: no input provided\n");
        } if (input_tokens.num_tokens == 1) {
            // check input is positive float
            if (!is_float_token(input_tokens.array[0])) {
                fprintf(stderr, "DEBUG: not a valid float\n");
                break;
            }
        } else if (((input_tokens.num_tokens + 1) % 4) == 0) {
            // check input in form A*part + B*part + ...
            if (!is_composite_form(&input_tokens)) {
                fprintf(stderr, "DEBUG: not in valid composite form\n");
                break;
            }
        } else {
            // invalid form
            fprintf(
                stderr,
                "DEBUG: not in valid composite form or valid float\n"
            );
            break;
        }

    }

    free_queue(&queue);
    free_allocator(&allocator);
    return 0;
}
