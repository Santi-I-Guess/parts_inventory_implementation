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
        int price;
        struct {
            int num_children;
            struct NODE *children;
        };
    };
} Node;

#endif
