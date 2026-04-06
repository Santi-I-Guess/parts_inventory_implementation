#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "node.h"
#include "queue.h"
#include "tokenizer.h"

// wrapper function
TokenArray get_input_tokens() {
    char input[80] = "";
    fgets(input, sizeof(input), stdin);
    input[strlen(input)-1] = '\0';
    remove_spaces(input);
    return tokenize_input(input);
}

int main() {
    // no reason for root to be malloc'd
    Node root = {0};
    Queue queue;
    init_queue(&queue);

    // enqueue first node
    fprintf(stderr, "what will you define?\n");
    TokenArray first_input_tokens = get_input_tokens();
    strcpy(root.name,first_input_tokens.tokens[0]);
    root.is_fully_defined = false;
    root.quantity = 1;
    enqueue(&queue, &root);

    while (queue.count > 0) {
        Node *curr_node = dequeue(&queue);
        fprintf(stderr, "what is %s?\n", curr_node->name);

        TokenArray input_tokens = get_input_tokens();

        // eventually replace this with empty queue check
#ifdef DEBUG
        if (strcmp(input_tokens.tokens[0], "quit") == 0)
            break;
#endif

#ifdef DEBUG
        for (int i = 0; i < input_tokens.num_tokens; ++i) {
            char (*curr_token)[32] = &input_tokens.tokens[i];
            printf("%s\n", *curr_token);
        }
#endif
    }

    free_queue(&queue);
    return 0;
}
