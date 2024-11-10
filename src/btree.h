#ifndef BTREE_H
#define BTREE_H

#include "table.h"

#define MAX_KEYS 3  

typedef struct BTreeNode {
    int num_keys;
    int keys[MAX_KEYS];
    Row values[MAX_KEYS];
    struct BTreeNode* children[MAX_KEYS + 1];
} BTreeNode;

BTreeNode* create_node();
void insert_key(BTreeNode* node, int key, Row* value);
Row* search_key(BTreeNode* node, int key);

#endif