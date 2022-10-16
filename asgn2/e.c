#include "mathlib.h"

#include <stdio.h>

static int computations = 0;
double e() {
    double previous = 1.0;
    double next = 1.0;
    double total = 2.0;
    double counter = 2.0;
    computations = 0;
    for (int i = 0; next > EPSILON; i += 1) {
        previous *= (counter);
        next = 1 / (double) previous;
        total = total + next;
        counter += 1;
        computations += 1;
    }
    return total;
}

int e_terms(void) {
    return computations;
}
