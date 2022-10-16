#include "bst.h"
#include <string.h>

//Initializing the global variable branches from bst.h
uint64_t branches = 0;

//Constructs an empty tree, which is NULL
Node *bst_create(void) {
    return NULL;
}

//The max and bst_height function is from lecture 18 Trees slides provided by Professor Long
static int max(int x, int y) {
    return x > y ? x : y;
}

uint32_t bst_height(Node *root) {
    if (root) {
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

uint32_t bst_size(Node *root) {
    if (root) {
        return 1 + bst_size(root->left) + bst_size(root->right);
    }
    return 0;
}

//The following functions are taken/referenced from lecture 18 Trees provided by Professor Long
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            return bst_find(root->right, oldspeak);
        }
    }
    return root;
}

//From pseudocode provided in Eugene's section
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root) {
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            root->left = bst_insert(root->left, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        } else if (strcmp(root->oldspeak, oldspeak) == 0) {
            return root;
        }
        return root;
    }
    //Creates and returns a node with the given oldspeak and newspeak if root equals NULL
    return node_create(oldspeak, newspeak);
}

//Performs an inorder traversal of the binary search tree to print
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
}

//Performs a postorder traversal of the binary search tree to delete
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
}
