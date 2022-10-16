#include "node.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        //If oldspeak equals NULL then return NULL because the key does not exist
        if (oldspeak == NULL) {
            return NULL;
        }
        n->oldspeak = strdup(oldspeak);
        //Have to account for when newspeak equals NULL
        //No newspeak is okay but calling strdup(NULL) results in a segfault
        if (newspeak == NULL) {
            n->newspeak = NULL;
        } else {
            n->newspeak = strdup(newspeak);
        }
        n->left = n->right = NULL;
        return n;
    }
    return NULL;
}

void node_delete(Node **n) {
    //Frees the memory of oldspeak and newspeak only if they exist
    if ((*n)->oldspeak != NULL) {
        free((*n)->oldspeak);
    }
    if ((*n)->newspeak != NULL) {
        free((*n)->newspeak);
    }
    free(*n);
    *n = NULL;
    return;
}
//Prints out oldspeak and its newspeak translation if they both exist
//Prints out only oldspeak if oldspeak exists and there is no newspeak translation
void node_print(Node *n) {
    if (n) {
        if (n->oldspeak != NULL && n->newspeak != NULL) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        }

        if (n->oldspeak != NULL && n->newspeak == NULL) {
            printf("%s\n", n->oldspeak);
        }
    }
    return;
}
