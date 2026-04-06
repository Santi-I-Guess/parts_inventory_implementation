#include <ctype.h>
#include <stdio.h> // DEBUG
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

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

// since input can be a float, allow periods as well
int is_number_part(int c) {
    return isdigit(c) || c == '.';
}

// tarray = token_array
void append_tarray(TokenArray *token_array, char token[32]) {
    char (*curr_token)[32] = &token_array->tokens[token_array->num_tokens];
    strcpy(*curr_token, token);
    token_array->num_tokens++;
}

TokenArray tokenize_input(const char input[80]) {
    TokenArray token_array = {0};

    size_t input_idx = 0;
    size_t input_len = strlen(input);
    while (input_idx < input_len) {
        char curr_char = input[input_idx];
        if (curr_char == '*' || curr_char == '+') {
            char token[32] = "";
            token[0] = curr_char;
            append_tarray(&token_array, token);
            input_idx++;
            continue;
        }

        int (*is_func)(int) = &isalpha;
        if (is_number_part(curr_char))
            is_func = &is_number_part;

        size_t token_len = 0;
        while (is_func(input[input_idx+token_len])) {
            token_len++;
        }

        if (token_len == 0) {
            input_idx += 1;
        } else {
            char token[32] = "";
            strncpy(token, &input[input_idx], token_len);
            append_tarray(&token_array, token);
            input_idx += token_len;
        }
    }
    return token_array;
}
