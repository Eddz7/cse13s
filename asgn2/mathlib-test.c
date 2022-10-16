#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

double diff(double first, double second) {
    double diff = first - second;
    diff = absolute(diff);
    return diff;
}
int main(int argc, char **argv) {

    bool test_e, test_n, test_b, test_r, test_m, test_v, test_a, test_s, test_h;
    test_e = test_n = test_b = test_r = test_m = test_v = test_a = test_s = test_h = false;

    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {

        switch (opt) {

        case 'e': test_e = true; break;

        case 'n': test_n = true; break;

        case 'b': test_b = true; break;

        case 'r': test_r = true; break;

        case 'm': test_m = true; break;

        case 'v': test_v = true; break;

        case 'a': test_a = true; break;

        case 's': test_s = true; break;

        case 'h': test_h = true; break;
        }
    }

    if (test_e || test_a) {
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, diff(e(), M_E));
        if (test_s) {
            printf("e_terms() = %d\n", e_terms());
        }
    }

    if (test_r || test_a) {
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), M_PI,
            diff(pi_euler(), M_PI));
        if (test_s) {
            printf("pi_euler_terms() = %d\n", pi_euler_terms());
        }
    }

    if (test_b || test_a) {
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), M_PI,
            diff(pi_bbp(), M_PI));
        if (test_s) {
            printf("pi_bbp_terms() = %d\n", pi_bbp_terms());
        }
    }

    if (test_m || test_a) {
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), M_PI,
            diff(pi_madhava(), M_PI));
        if (test_s) {
            printf("pi_madhava_terms() = %d\n", pi_madhava_terms());
        }
    }

    if (test_v || test_a) {
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), M_PI,
            diff(pi_viete(), M_PI));
        if (test_s) {
            printf("pi_viete_terms() = %d\n", pi_viete_factors());
        }
    }

    if (test_n || test_a) {
        for (double i = 0.0; i <= 10.0; i += 0.1) {
            printf("sqrt_newton(%f) = %16.15lf, sqrt(%f) = %16.15lf, diff = (%16.15lf)\n", (i),
                sqrt_newton(i), (i), sqrt(i), diff(sqrt_newton(i), sqrt(i)));
            if (test_s) {
                printf("sqrt_newton_terms() = %d\n", sqrt_newton_iters());
            }
        }
    }

    if (argc == 1 || test_h) {
        printf("SYNOPSIS\n"
               "   A test harness for the small numerical library.\n\n"
               "USAGE\n"
               "   ./mathlib-test [aebmrvnsh]\n\n"
               "OPTIONS\n"
               "  -a   Runs all tests.\n"
               "  -e   Runs e test.\n"
               "  -b   Runs BBP pi test.\n"
               "  -m   Runs Madhava pi test.\n"
               "  -r   Runs Euler pi test.\n"
               "  -v   Runs Viete pi test.\n"
               "  -n   Runs Newton square root tests.\n"
               "  -s   Print verbose statistics.\n"
               "  -h   Display program synopsis and usage.\n");
    }
    return 0;
}
