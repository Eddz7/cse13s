#include "mathlib.h"

#include <stdio.h>

static int computations = 0;

double pi_madhava(void) {
    computations = 0;
    double next = 1.0;
    double total = 0.0;
    double denom = 1.0;
    for (int i = 0; absolute(denom) > EPSILON; i += 1) {
        next *= (-3);
        if (i == 0) {
            next = 1;
        }
        denom = (1 / (double) (next * (2 * i + 1)));
        total += denom;
        computations += 1;
    }
    total *= sqrt_newton(12.0);
    return total;
}

int pi_madhava_terms(void) {
    return computations;
}
