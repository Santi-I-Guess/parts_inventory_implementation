#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allocator.h"
#include "node.h"
#include "tokenizer.h"

// wrapper function
TokenArray tokenize_input() {
    char input[80] = "";
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1] = '\0';
    remove_spaces(input);
    return tokenize_line(input);
}

void setup_root_node(Node *root) {
    fprintf(stderr, "what will you define?\n");
    TokenArray tokens = tokenize_input();
    strcpy(root->name, tokens.array[0]);
    root->quantity = 1;
}

void define_node(Node *curr_node, NodeAllocator *allocator) {
    fprintf(stderr, "what is %s?\n", curr_node->name);

    TokenArray input_tokens = tokenize_input();

    /*
#ifdef DEBUG
    for (int i = 0; i < input_tokens.num_tokens; ++i) {
        char (*curr_token)[32] = &input_tokens.array[i];
        fprintf(stderr, "DEBUG: token \"%s\"\n", *curr_token);
    }
#endif
    */

    if (input_tokens.num_tokens == 0) {
        // no input
        fprintf(stderr, "DEBUG: no input provided\n");
    } if (input_tokens.num_tokens == 1) {
        // check input is positive float
        if (!is_float_token(input_tokens.array[0])) {
            fprintf(stderr, "DEBUG: not a valid float\n");
            return;
        }
        curr_node->node_type = PRICE;
        curr_node->price = strtof(input_tokens.array[0], NULL);
    } else if (((input_tokens.num_tokens + 1) % 4) == 0) {
        // check input in form A*part + B*part + ...
        if (!is_composite_form(&input_tokens)) {
            fprintf(stderr, "DEBUG: not in valid composite form\n");
            return;
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
            define_node(child, allocator);
        }
    } else {
        // invalid form
        fprintf(
            stderr,
            "DEBUG: not in valid composite form or valid float\n"
        );
        return;
    }
}

void print_tree(Node *curr, int level) {
    char buffer[256] = "";
    for (int i = 0; i < level; ++i) {
        sprintf(&buffer[2*i], "  ");
    }
    sprintf(&buffer[2*level], "|-");
    sprintf(&buffer[strlen(buffer)], "%s x %d", curr->name, curr->quantity);

    if (curr->node_type == PRICE) {
        sprintf(&buffer[strlen(buffer)], " ($%.2f)", curr->price);
        printf("%s\n", buffer);
    } else {
        printf("%s\n", buffer);
        for (int i = 0; i < curr->num_children; ++i) {
            print_tree(curr->children[i], level+1);
        }
    }
}

float get_price(Node *curr, int multiplier) {
    if (curr->node_type == PRICE)
        return curr->price * (float)multiplier;

    float running_total = 0;
    for (int i = 0; i < curr->num_children; ++i) {
        Node *curr_child = curr->children[i];
        running_total += get_price(curr_child, curr_child->quantity);
    }
    return running_total * (float)multiplier;
}

int main() {
    NodeAllocator allocator;
    init_allocator(&allocator);
    Node *root = alloc_new_node(&allocator);

    setup_root_node(root);
    define_node(root, &allocator);
    float total_price = get_price(root, 1);
    printf("total cost for bike is %.2f\n", total_price);

    print_tree(root, 0);

    free_allocator(&allocator);
    return 0;
}
