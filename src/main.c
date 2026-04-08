#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
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
void setup_root_node(Node *root) {
    fprintf(stderr, "what will you define?\n");
    TokenArray tokens = tokenize_input();
    strcpy(root->name, tokens.array[0]);
    root->quantity = 1;
}

// define all parts with either a price or composition of parts
void fill_in_tree(Node *root, NodeAllocator *allocator) {
    Queue definition_queue;
    init_queue(&definition_queue);
    setup_root_node(root);

    enqueue(&definition_queue, root);
    while (definition_queue.count > 0) {
        Node *curr_node = dequeue(&definition_queue);
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
            curr_node->node_type = PRICE;
            curr_node->price = strtof(input_tokens.array[0], NULL);
        } else if (((input_tokens.num_tokens + 1) % 4) == 0) {
            // check input in form A*part + B*part + ...
            if (!is_composite_form(&input_tokens)) {
                fprintf(stderr, "DEBUG: not in valid composite form\n");
                break;
            }

            int num_children = (input_tokens.num_tokens + 1) / 4;
            curr_node->node_type = COMPOSITE;
            curr_node->children = calloc(num_children, sizeof(Node*));
            curr_node->num_children = 0; // 0 for now, use as idx

            for (int word_idx = 2; word_idx < input_tokens.num_tokens; word_idx += 4) {
                // parse relevant tokens
                char (*coefficient_token)[32] = &input_tokens.array[word_idx-2];
                char (*word_token)[32] = &input_tokens.array[word_idx];
                int coefficient = atoi(*coefficient_token);

                // create new node, set quantity and name, add as child to curr node
                Node *child = alloc_new_node(allocator);
                strcpy(child->name, *word_token);
                child->quantity = coefficient;

                // add new node as child of curr_node, enqueue child
                curr_node->children[curr_node->num_children] = child;
                curr_node->num_children++;
                enqueue(&definition_queue, child);
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

    free_queue(&definition_queue);
}

int main() {
    NodeAllocator allocator;
    init_allocator(&allocator);
    Node *root = alloc_new_node(&allocator);

    fill_in_tree(root, &allocator);

    free_allocator(&allocator);
    return 0;
}
