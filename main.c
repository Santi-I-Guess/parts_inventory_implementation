#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// removes spaces, writes to input
void remove_spaces(char input[80]) {
    size_t stripped_idx = 0;
    char stripped_input[80] = "";

    size_t original_len = strlen(input);
    for (size_t i = 0; i < original_len; ++i) {
        if (input[i] != ' ') {
            stripped_input[stripped_idx] = input[i];
            stripped_idx++;
        }
    }

    // since stripped_input is assumed to be <= input in length,
    // we need to clear input to avoid leftover chars
    memset(input, 0, original_len * sizeof(char));
    strcpy(input, stripped_input);
}

// for now, just finds the token and prints it on newline
void tokenize_input(const char input[80]) {
    char input_copy[80] = "";
    strcpy(input_copy, input);
    remove_spaces(input_copy);

    size_t input_idx = 0;
    size_t input_len = strlen(input);
    while (input_idx < input_len) {
        size_t token_idx = 0;
        char token[80] = "";
        char curr_char = input_copy[input_idx];
        if (curr_char == '*' || curr_char == '+') {
            printf("%c\n", curr_char);
            input_idx++;
            continue;
        }

        int (*is_func)(int) = &isalpha;
        if (isdigit(curr_char))
            is_func = &isdigit;

        while (is_func(input_copy[input_idx+token_idx])) {
            token[token_idx] = input_copy[input_idx+token_idx];
            token_idx++;
        }
        switch (token_idx) {
            case 0:
                input_idx += 1;
                break;
            default:
                printf("%s\n", token);
                input_idx += token_idx;
                break;
        }
    }
}

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
