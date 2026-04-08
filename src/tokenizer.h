#ifndef TOKENIZER_H
#define TOKENIZER_H 1

#include <stdbool.h>

typedef struct {
    int num_tokens;
    char array[32][32];
} TokenArray;

void remove_spaces(char input[80]);
int is_integer_char(int c);
bool is_integer_token(const char token[32]);
bool is_float_token(const char token[32]);
bool is_part_name_token(const char token[32]);
bool is_composite_form(TokenArray *token_array);

void append_tarray(TokenArray *token_array, char token[32]);
TokenArray tokenize_line(const char line[80]);

#endif
