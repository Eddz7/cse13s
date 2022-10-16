#include "bv.h"
#include "bf.h"
#include "speck.h"
#include "salts.h"
#include <stdlib.h>
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

//Constructor for a bloom filter, the bit vector filter is the array of bits in the bloom filter
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
    }
    bf->filter = bv_create(size);
    if (bf->filter) {
        return bf;
    }
    return NULL;
}

void bf_delete(BloomFilter **bf) {
    free(*bf);
    *bf = NULL;
    return;
}

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

//Inserts oldspeak into the bloom filter
//Hashes the oldspeak with each of the three salts using the hash function from speck.c
//Sets the bits at those indices for the bit vector filter in the bloom filter
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t first, second, third;
    //Set variables to their respective indices returned from hashing the salts with oldspeak
    first = hash(bf->primary, oldspeak);
    second = hash(bf->secondary, oldspeak);
    third = hash(bf->tertiary, oldspeak);
    //Set the bits at the indices returned from hash in the bit vector filter
    bv_set_bit(bf->filter, (first % bf_size(bf)));
    bv_set_bit(bf->filter, (second % bf_size(bf)));
    bv_set_bit(bf->filter, (third % bf_size(bf)));
}

//Probes the bloom filter and returns true if all the bits at the indices are set
//Returning true does not guarantee oldspeak was added, it is probabilistic
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    //Same process as bf_insert
    uint32_t first, second, third;
    bool firstset, secondset, thirdset;
    //Set the variables first, second, and third to the index of the key from hash in speck
    first = hash(bf->primary, oldspeak);
    second = hash(bf->secondary, oldspeak);
    third = hash(bf->tertiary, oldspeak);
    //Set the booleans to the results of bv_get_bit, if all are true, return true
    firstset = bv_get_bit(bf->filter, (first % bf_size(bf)));
    secondset = bv_get_bit(bf->filter, (second % bf_size(bf)));
    thirdset = bv_get_bit(bf->filter, (third % bf_size(bf)));
    if (firstset && secondset && thirdset) {
        return true;
    }
    return false;
}

//Returns the number of set bits in the bloom filter
uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bf_size(bf); i += 1) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
}
