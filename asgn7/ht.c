#include "ht.h"
#include "salts.h"
#include "bst.h"
#include "speck.h"
#include <stdlib.h>
#include <string.h>

//Initializing the global variable lookups defined in ht.h
uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

//Constructor for the hash table
//The array of nodes, trees, is the binary search tree to be allocated
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
        ht->size = size;
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        if (!ht->trees) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(HashTable **ht) {
    free((*ht)->trees);
    free(*ht);
    *ht = NULL;
    return;
}

uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

//From lecture 28 hashing lecture slides provided by Professor Long
//From pseudocode in Eugene's section
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1;
    uint32_t hash_index;
    //Get the index of the binary search tree using hash from speck.h
    //Mod the hash value with size of the hash table so the returned index is in range
    hash_index = (hash(ht->salt, oldspeak) % ht->size);
    return bst_find(ht->trees[hash_index], oldspeak);
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups += 1;
    uint32_t hash_index;
    //Get the index of the binary search tree like in ht_lookup
    //Mod the hash value with size of the hash table so the returned index is in range
    hash_index = (hash(ht->salt, oldspeak) % ht->size);
    //Update the node at the index to the node returned from inserting the node using bst_insert
    ht->trees[hash_index] = bst_insert(ht->trees[hash_index], oldspeak, newspeak);
    return;
}

//Loops through the hash table and returns the number of non-NULL binary search trees
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i] != NULL) {
            count += 1;
        }
    }
    return count;
}

//Computes the average binary search tree size in the hash table
//Adds the sizes of all the bsts and divides by the number returned from ht_count
double ht_avg_bst_size(HashTable *ht) {
    uint32_t size_sum = 0;
    uint32_t divisor = ht_count(ht);
    for (uint32_t i = 0; i < ht->size; i += 1) {
        size_sum += (double) bst_size(ht->trees[i]);
    }
    double avg = (double) size_sum / divisor;
    return avg;
}

//Computes the average binary search tree height in the hash table
//Adds the heights of all the bsts and divides by the number returned from ht_count
double ht_avg_bst_height(HashTable *ht) {
    uint32_t height_sum = 0;
    uint32_t divisor = ht_count(ht);
    for (uint32_t i = 0; i < ht->size; i += 1) {
        height_sum += (double) bst_height(ht->trees[i]);
    }
    double avg = (double) height_sum / divisor;
    return avg;
}

//Prints out the contents of the hash table
void ht_print(HashTable *ht) {
    for (uint32_t i = 0; i < ht->size; i += 1) {
        bst_print(ht->trees[i]);
    }
    return;
}
