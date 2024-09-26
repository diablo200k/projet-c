#include "btree.h"
#include <stdlib.h>



BTreeNode* create_node() {
    BTreeNode* new_node = malloc(sizeof(BTreeNode));
    new_node->num_keys = 0;
    for (int i = 0; i < MAX_KEYS; i++) {
        new_node->children[i] = NULL;
    }
    return new_node;
}

void insert_key(BTreeNode* node, int key, Row* value) {
   
    int i = node->num_keys - 1;
    while (i >= 0 && node->keys[i] > key) {
        node->keys[i + 1] = node->keys[i];
        node->values[i + 1] = node->values[i];
        i--;
    }
    node->keys[i + 1] = key;
    node->values[i + 1] = *value;
    node->num_keys++;
}

Row* search_key(BTreeNode* node, int key) {
    int i = 0;
    while (i < node->num_keys && key > node->keys[i]) {
        i++;
    }
    if (i < node->num_keys && key == node->keys[i]) {
        return &node->values[i];
    }
    if (node->children[i] == NULL) {
        return NULL;
    }
    return search_key(node->children[i], key);
}