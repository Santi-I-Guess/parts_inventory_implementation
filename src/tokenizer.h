#ifndef TOKENIZER_H
#define TOKENIZER_H 1

#define TOKEN_LEN 80

// a string view (or slice in zig) is just a pointer and an integer,
// which makes it much smaller to store tokens and trivial to copy
typedef struct {
    int len;
    const char *data;
} StringView;

typedef struct {
    int num_tokens;
    StringView tokens[32];
} TokenArray;

void remove_spaces(char input[TOKEN_LEN]);
int is_number_part(int c);
void append_tarray(TokenArray *token_array, StringView token);
TokenArray tokenize_input(const char input[TOKEN_LEN]);

#endif
