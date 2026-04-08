#include <ctype.h>
#include <stdbool.h>
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
int is_integer_char(int c) {
    return isdigit(c) || c == '.';
}

bool is_integer_token(const char token[32]) {
    size_t token_len = strlen(token);
    if (token_len == 0)
        return false;
    for (size_t i = 0; i < token_len; ++i) {
        char curr_char = token[i];
        if (!isdigit(curr_char)) {
            return false;
        }
    }
    return true;
}

bool is_float_token(const char token[32]) {
    bool seen_period = false;
    size_t token_len = strlen(token);
    if (token_len == 0)
        return false;
    for (size_t i = 0; i < token_len; ++i) {
        char curr_char = token[i];
        if (curr_char == '.' && !seen_period) {
            seen_period = true;
        } else if (curr_char == '.') {
            return false;
        } else if (!isdigit(curr_char)) {
            return false;
        }
    }
    return true;
}

bool is_part_name_token(const char token[32]) {
    size_t token_len = strlen(token);
    if (token_len == 0)
        return false;
    for (size_t i = 0; i < token_len; ++i) {
        char curr_char = token[i];
        if (!isalpha(curr_char)) {
            return false;
        }
    }
    return true;
}

bool is_composite_form(TokenArray *token_array) {
    enum {
        COEFFICIENT,
        MULTIPLY,
        NAME,
        ADD
    } token_state = COEFFICIENT;

    // can't break out of loop inside switch, so use variable
    bool is_valid = true;
    for (int i = 0; i < token_array->num_tokens; ++i) {
        if (!is_valid)
            return false;

        char (*curr_token)[32] = &token_array->array[i];
        switch (token_state) {
            case COEFFICIENT:
                if (!is_integer_token(*curr_token))
                    is_valid = false;
                token_state = MULTIPLY;
                break;
            case MULTIPLY:
                if (strlen(*curr_token) != 1 || *curr_token[0] != '*')
                    is_valid = false;
                token_state = NAME;
                break;
            case NAME:
                if (!is_part_name_token(*curr_token))
                    is_valid = false;
                token_state = ADD;
                break;
            case ADD:
                if (strlen(*curr_token) != 1 || *curr_token[0] != '+')
                    is_valid = false;
                token_state = COEFFICIENT;
                break;
        }
    }

    return is_valid;
}

// tarray = token_array
void append_tarray(TokenArray *token_array, char token[32]) {
    char (*curr_token)[32] = &token_array->array[token_array->num_tokens];
    strcpy(*curr_token, token);
    token_array->num_tokens++;
}

TokenArray tokenize_line(const char line[80]) {
    TokenArray token_array = {0};

    size_t input_idx = 0;
    size_t input_len = strlen(line);
    while (input_idx < input_len) {
        char curr_char = line[input_idx];
        if (curr_char == '*' || curr_char == '+') {
            char token[32] = "";
            token[0] = curr_char;
            append_tarray(&token_array, token);
            input_idx++;
            continue;
        }

        int (*is_func)(int) = &isalpha;
        if (is_integer_char(curr_char))
            is_func = &is_integer_char;

        size_t token_len = 0;
        while (is_func(line[input_idx+token_len])) {
            token_len++;
        }

        if (token_len == 0) {
            input_idx += 1;
        } else {
            char token[32] = "";
            strncpy(token, &line[input_idx], token_len);
            append_tarray(&token_array, token);
            input_idx += token_len;
        }
    }
    return token_array;
}
