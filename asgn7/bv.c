#include "bv.h"
#include <stdlib.h>
#include <stdio.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

//Constructor for a bit vector that holds length bits
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        //From bv8.h in the code comments repository provided by the professor
        uint32_t bytes = length / 8 + (length % 8 ? 1 : 0);
        //Need to use calloc to allocate memory for the vector to initialize bits to 0
        bv->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
        if (!bv->vector) {
            return NULL;
        }
        return bv;
    }
    return NULL;
}

//Destructor for the BitVector, frees bv and bv->vector
void bv_delete(BitVector **bv) {
    free((*bv)->vector);
    free(*bv);
    *bv = NULL;
    return;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

//Set bit, clear bit, and get bit are from bv8.h in the code comments provided by the professor
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] |= (0x1 << i % 8);
    return true;
}

bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] &= ~(0x1 << i % 8);
    return true;
}

bool bv_get_bit(BitVector *bv, uint32_t i) {
    bool bit = (bv->vector[i / 8] >> i % 8) & 0x1;
    if (i > bv->length || bit == 0) {
        return false;
    }
    return true;
}

void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i += 1) {
        printf("%d\n", (bv->vector[i / 8] >> i % 8) & 0x1);
    }
    return;
}
