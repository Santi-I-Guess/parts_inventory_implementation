#ifndef NODE_H
#define NODE_H 1

typedef struct NODE {
    bool is_fully_defined;
    char name[80];
    int quantity;
    int node_type; // 0 for "composite", 1 for "basic"
    union {
        int price;
        struct {
            int num_children;
            struct NODE *children;
        };
    };
} Node;

#endif
