#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "tokenizer.h"
#include "node.h"

int main() {
    Node *root = NULL;

    while (true) {
        char line[80] = "";
        printf("> ");
        fgets(line, sizeof(line), stdin);
        line[strlen(line)-1] = '\0';
        remove_spaces(line);

        // eventually replace this with empty queue check
#ifdef DEBUG
        if (strcmp(line, "quit") == 0)
            break;
#endif

        TokenArray input_tokens = tokenize_input(line);
#ifdef DEBUG
        for (int i = 0; i < input_tokens.num_tokens; ++i) {
            StringView curr_token = input_tokens.tokens[i];
            printf("%.*s\n", curr_token.len, curr_token.data);
        }
#endif
    }

    return 0;
}
