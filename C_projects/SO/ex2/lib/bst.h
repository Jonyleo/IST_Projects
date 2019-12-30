/* bst.h */
#ifndef BST_H
#define BST_H
#include <stdio.h>

#define DELAY 5000

typedef struct node {
    char* key;
    int inumber;

    struct node* left;
    struct node* right;
} node;

void insertDelay(int cycles);
node *search(node *p, char* key);
node *insert(node *p, char* key, int inumber);
node *find_min(node *p);
node *remove_min(node *p);
node *remove_item(node *p, char* key);
void free_tree(node *p);
void print_tree(FILE* fp, node *p);

#endif /* BST_H */
