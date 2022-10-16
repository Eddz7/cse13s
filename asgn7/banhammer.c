#include "ht.h"
#include "bf.h"
#include "node.h"
#include "bst.h"
#include "parser.h"
#include "messages.h"
#include <stdio.h>
#include <regex.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#define OPTIONS                   "ht:f:s"
#define default_hash_table_size   65536
#define default_bloom_filter_size 1048576
#define REGEX_WORD                "[a-zA-Z0-9_]+"
int main(int argc, char **argv) {
    //Make booleans for the command line options and for which messages to print
    bool h_test, s_test, thoughtcrime, counseling;
    h_test = s_test = thoughtcrime = counseling = false;
    char oldspeak[1024];
    char newspeak[1024];
    uint64_t hash_size, bloom_size;
    //Make char pointers to read in integers from optarg
    char *hash_ptr, *bloom_ptr;
    hash_size = default_hash_table_size;
    bloom_size = default_bloom_filter_size;
    Node *n, *old_print, *old_and_new_print;
    old_print = old_and_new_print = NULL;
    FILE *bad_speak, *new_speak;
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 's': s_test = true; break;
        case 'h': h_test = true; break;
        case 't': hash_size = strtoul(optarg, &hash_ptr, 10); break;
        case 'f': bloom_size = strtoul(optarg, &bloom_ptr, 10); break;
        }
    }
    if (h_test) {
        printf("SYNOPSIS\n"
               "  A word filtering program for the GPRSC.\n"
               "  Filters out and reports bad words parsed from stdin.\n\n"
               "USAGE\n"
               "  ./banhammer [-hs] [-t size] [-f size]\n\n"
               "OPTIONS\n"
               "  -h\t       Program usage and help.\n"
               "  -s\t       Print program statistics.\n"
               "  -t size      Specify hash table size (default: 2^16).\n"
               "  -f size      Specify Bloom filter size (default: 2^20).\n");
        return 0;
    }
    //Initialize the bloom filter and hash table using default or inputted sizes
    BloomFilter *bf = bf_create(bloom_size);
    HashTable *ht = ht_create(hash_size);
    //Open badspeak.txt and insert the list of oldspeak words into the bloom filter and hash table
    bad_speak = fopen("badspeak.txt", "r");
    while (fscanf(bad_speak, "%s\n", oldspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, NULL);
    }
    //Open newspeak.txt and insert only the list of oldspeak words into the bloom filter
    //Insert both the list of oldspeak and newspeak words into the hash table
    new_speak = fopen("newspeak.txt", "r");
    while (fscanf(new_speak, "%s %s\n", oldspeak, newspeak) != EOF) {
        bf_insert(bf, oldspeak);
        ht_insert(ht, oldspeak, newspeak);
    }
    //Referenced from the regex example provided in the assignment doc
    regex_t re;
    if (regcomp(&re, REGEX_WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }
    char *word = NULL;
    while ((word = next_word(stdin, &re)) != NULL) {
        //If bf_probe returns true that means further action needs to be taken
        if (bf_probe(bf, word)) {
            n = ht_lookup(ht, word);
            if (n) {
                if (n->newspeak == NULL) {
                    thoughtcrime = true;
                    old_print = bst_insert(old_print, word, NULL);
                }
                if (n->newspeak != NULL) {
                    counseling = true;
                    old_and_new_print = bst_insert(old_and_new_print, word, n->newspeak);
                }
            }
        }
    }
    if (s_test) {
        printf("Average BST size: %f\n", (double) ht_avg_bst_size(ht));
        printf("Average BST height: %f\n", (double) ht_avg_bst_height(ht));
        printf("Average branches traversed: %f\n", (double) branches / lookups);
        printf("Hash table load: %f%%\n", (double) (100 * (double) ht_count(ht) / ht_size(ht)));
        printf("Bloom filter load: %f%%\n", (double) (100 * (double) bf_count(bf) / bf_size(bf)));
        return 0;
    }
    //Print messages accordingly depending on the booleans thoughtcrime and counseling
    if (thoughtcrime && counseling) {
        printf("%s", mixspeak_message);
        if (old_print != NULL && old_and_new_print != NULL) {
            bst_print(old_print);
            bst_print(old_and_new_print);
        }
    }
    if (thoughtcrime && counseling == false) {
        printf("%s", badspeak_message);
        if (old_print != NULL) {
            bst_print(old_print);
        }
    }
    if (counseling && thoughtcrime == false) {
        printf("%s", goodspeak_message);
        if (old_and_new_print != NULL) {
            bst_print(old_and_new_print);
        }
    }
    bf_delete(&bf);
    ht_delete(&ht);
    clear_words();
    regfree(&re);
    fclose(bad_speak);
    fclose(new_speak);
    return 0;
}
