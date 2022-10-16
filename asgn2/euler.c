#include "mathlib.h"

#include <stdio.h>

static int computations = 0;

double pi_euler(void) {
    double next = 1.0;
    double term = 1.0;
    double total = 0.0;
    computations = 0;
    for (int i = 1; next > EPSILON; i += 1) {
        term = i;
        term *= i;
        next = 1 / term;
        total += next;
        computations += 1;
    }
    total = sqrt_newton(total * 6);
    return total;
}

int pi_euler_terms(void) {
    return computations;
}
