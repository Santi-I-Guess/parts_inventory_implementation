#ifndef NODE_H
#define NODE_H 1

typedef struct NODE {
    char name[80];
    int quantity;
    enum {
        PRICE,
        COMPOSITE
    } node_type;
    union {
        float price;
        struct {
            int num_children;
            struct NODE **children;
        };
    };
} Node;

void print_tree(Node *curr, int level);
float get_price(Node *curr, int multiplier);

#endif
