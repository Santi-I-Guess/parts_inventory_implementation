#include <stdio.h>
#include <string.h>

#include "node.h"

float get_price(Node *curr, int multiplier) {
    if (curr->node_type == PRICE)
        return curr->price * (float)multiplier;

    float running_total = 0;
    for (int i = 0; i < curr->num_children; ++i) {
        Node *curr_child = curr->children[i];
        running_total += get_price(curr_child, curr_child->quantity);
    }
    return running_total * (float)multiplier;
}

void print_tree(Node *curr, int level) {
    char buffer[256] = "";
    for (int i = 0; i < level; ++i) {
        sprintf(&buffer[2*i], "  ");
    }
    sprintf(&buffer[2*level], "|-");
    sprintf(&buffer[strlen(buffer)], "%s x %d", curr->name, curr->quantity);

    if (curr->node_type == PRICE) {
        sprintf(&buffer[strlen(buffer)], " ($%.2f)", curr->price);
        printf("%s\n", buffer);
    } else {
        sprintf(&buffer[strlen(buffer)], " ($%.2f) (composite)", get_price(curr, 1));
        printf("%s\n", buffer);
        for (int i = 0; i < curr->num_children; ++i) {
            print_tree(curr->children[i], level+1);
        }
    }
}
