#include <ctype.h>
#include <stdio.h> // DEBUG
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"

// removes spaces, writes to input
void remove_spaces(char input[TOKEN_LEN]) {
    size_t stripped_idx = 0;
    char stripped_input[TOKEN_LEN] = "";

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
void append_tarray(
    TokenArray *token_array,
    StringView token
) {
    token_array->tokens[token_array->num_tokens] = token;
    token_array->num_tokens++;
}

TokenArray tokenize_input(const char input[TOKEN_LEN]) {
    TokenArray token_array = {0};

    size_t input_idx = 0;
    size_t input_len = strlen(input);
    while (input_idx < input_len) {
        char curr_char = input[input_idx];
        if (curr_char == '*' || curr_char == '+') {
            StringView token = { .data = &input[input_idx], .len = 1 };
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
            StringView token = { .data = &input[input_idx], .len = token_len };
            append_tarray(&token_array, token);
            input_idx += token_len;
        }
    }
    return token_array;
}
