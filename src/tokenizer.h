#ifndef TOKENIZER_H
#define TOKENIZER_H 1

typedef struct {
    int num_tokens;
    char array[32][32];
} TokenArray;

void remove_spaces(char input[80]);
int is_number_part(int c);
void append_tarray(TokenArray *token_array, char token[32]);
TokenArray tokenize_line(const char line[80]);

#endif
