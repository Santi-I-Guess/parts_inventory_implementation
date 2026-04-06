#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

typedef struct {
    size_t num_children;
    struct NODE *children;
} CompositeNode;

typedef struct {
    int value;
} ValueNode;

typedef struct NODE {
    char name[80];
    int quantity;
    int node_type; // 0 for Composite, 1 for ValueNode
    union {
        CompositeNode *composite_node;
        ValueNode *value_node;
    };
} Node;

int main() {
    Node *root = NULL;
    Node *curr = root;

    while (true) {
        char line[80] = "";
        printf("> ");
        fgets(line, sizeof(line), stdin);
        line[strlen(line)-1] = '\0';
        tokenize_input(line);
    }

    return 0;
}
