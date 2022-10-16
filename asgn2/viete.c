#include "mathlib.h"

#include <stdio.h>

static int computations = 0;

double pi_viete(void) {
    computations = 0;
    double previous = 1.0;
    double total = 1.0;
    double next = 2.0;
    for (int i = 1; absolute((1 - next)) > EPSILON; i += 1) {
        previous = sqrt_newton(2.0 + previous);
        if (i == 1) {
            previous = sqrt_newton(2.0);
        }
        next = previous / (double) 2;
        total *= next;
        computations += 1;
    }
    total = 2 / (double) total;
    return total;
}

int pi_viete_factors(void) {
    return computations;
}
