#include "mathlib.h"

#include <stdio.h>

static int computations = 0;
double pi_bbp(void) {
    computations = 0;
    double next = 1.0;
    double total = 0.0;
    for (int i = 0; next > EPSILON; i += 1) {
        next = next * 1 / (double) 16;
        if (i == 0) {
            next = 1;
        }
        total = total
                + (next
                    * (4 / (double) (8 * i + 1) - 2 / (double) (8 * i + 4)
                        - 1 / (double) (8 * i + 5) - 1 / (double) (8 * i + 6)));
        computations += 1;
    }
    return total;
}

int pi_bbp_terms(void) {
    return computations;
}
