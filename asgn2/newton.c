#include "mathlib.h"

#include <stdio.h>

static int computations = 0;

double sqrt_newton(double x) {
    computations = 0;
    double z = 0.0;
    double y = 1.0;
    while (absolute(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + x / z);
        computations += 1;
    }
    return y;
}

int sqrt_newton_iters(void) {
    return computations;
}
