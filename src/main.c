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
    char name[TOKEN_LEN];
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
        remove_spaces(line);

        // DEBUG
        if (strcmp(line, "quit") == 0)
            break;

        TokenArray input_tokens = tokenize_input(line);
        for (int i = 0; i < input_tokens.num_tokens; ++i) {
            StringView curr_token = input_tokens.tokens[i];
            printf("%.*s\n", curr_token.len, curr_token.data);
        }
    }

    return 0;
}
