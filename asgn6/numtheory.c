#include "numtheory.h"
#include "randstate.h"
//Went to Christan's section where he shared ideas and pseudocode
//Set mpz variables
//Follow pseudocode, calling mpz functions as necessary
//Remember to clear all mpz variables

//Computes the greatest common divisor of a and b
//Stores the value of the computed divisor in d
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t alpha, beta;
    mpz_inits(alpha, beta, NULL);
    mpz_set(alpha, a);
    mpz_set(beta, b);
    //The while loop below implements the Euclidean algorithm
    //Computes the remainder which is faster than subtraction
    while (mpz_cmp_ui(beta, 0) != 0) {
        mpz_set(d, beta);
        mpz_mod(beta, alpha, beta);
        mpz_set(alpha, d);
    }
    mpz_set(d, alpha);
    mpz_clears(alpha, beta, NULL);
    return;
}
//Computes the inverse i of a modulo n
//If a modular inverse cannot be found, set i to 0
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    mpz_t r, dashr, t, dasht, q, mul_temp, temp_r, temp_t;
    mpz_inits(r, dashr, t, dasht, q, mul_temp, temp_r, temp_t, NULL);
    mpz_set(r, n);
    mpz_set(dashr, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(dasht, 1);
    while (mpz_cmp_ui(dashr, 0) != 0) {
        mpz_fdiv_q(q, r, dashr);
        //Temporary values are needed to store the original value of a var
        //The original values would be overwritten and assignment would be incorrect otherwise
        //This is necessary for parallel assignment which is simultaneous
        mpz_set(temp_r, dashr);
        //stores and saves the original value of r'
        mpz_mul(mul_temp, q, dashr);
        mpz_sub(dashr, r, mul_temp);
        mpz_set(r, temp_r);
        //assigns the original value of r' to r after assignment
        //same process for parallel assignment of t and t'
        mpz_set(temp_t, dasht);
        mpz_mul(mul_temp, q, dasht);
        mpz_sub(dasht, t, mul_temp);
        mpz_set(t, temp_t);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(i, 0);
        mpz_clears(r, dashr, t, dasht, q, mul_temp, temp_r, temp_t, NULL);
        return;
    }
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    mpz_set(i, t);
    mpz_clears(r, dashr, t, dasht, q, mul_temp, temp_r, temp_t, NULL);
    return;
}
//Performs fast modular exponentiation
//Computes base raised to the exponent power modulo modulus
//Stores the computed result in out
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t modulus) {
    mpz_t v, p, mul_temp, mod_temp, expo_temp;
    mpz_inits(v, p, mul_temp, mod_temp, expo_temp, NULL);
    mpz_init_set_ui(v, 1);
    mpz_init_set(p, base);
    mpz_set(expo_temp, exponent);
    while (mpz_cmp_ui(expo_temp, 0) > 0) {
        mpz_mod_ui(mod_temp, expo_temp, 2);
        if (mpz_cmp_ui(mod_temp, 0) != 0) {
            mpz_mul(mul_temp, v, p);
            mpz_mod(v, mul_temp, modulus);
        }
        mpz_mul(mul_temp, p, p);
        mpz_mod(p, mul_temp, modulus);
        mpz_fdiv_q_ui(expo_temp, expo_temp, 2);
    }
    mpz_set(out, v);
    mpz_clears(v, p, mul_temp, mod_temp, expo_temp, NULL);
    return;
}
//Uses the Miller-Rabin primality test to indicate whether or not a number n is prime
//Uses iters number of Miller-Rabin iterations
//Can only say a number is probably prime but with a strong probability of being right
//The chances of being wrong is (1/4) raised to the power of iters
bool is_prime(mpz_t n, uint64_t iters) {
    mpz_t a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent;
    mpz_inits(a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent, NULL);
    mpz_set_ui(s, 0);
    mpz_sub_ui(r, n, 1);
    mpz_set(n_minus_one, r);
    mpz_set_ui(exponent, 2);
    mpz_mod_ui(temp, n, 2);
    //Have to account for all the base cases in the Miller-Rabin test
    //Base case of 2, only even number that is prime
    if (mpz_cmp_ui(n, 2) == 0) {
        mpz_clears(a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent, NULL);
        return true;
    }
    //The number n can't be prime if n is even except for the base case of 2
    //Numbers less than 2 must also be accounted for
    if (mpz_cmp_ui(temp, 0) == 0 || mpz_cmp_ui(n, 2) < 0) {
        mpz_clears(a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent, NULL);
        return false;
    }
    //Have to account for the base case of 3
    //It is too small in the range of random numbers generated for testing
    if (mpz_cmp_ui(n, 3) == 0) {
        mpz_clears(a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent, NULL);
        return true;
    }
    mpz_mod_ui(temp, r, 2);
    while (mpz_cmp_ui(temp, 0) == 0) {
        mpz_add_ui(s, s, 1);
        mpz_fdiv_q_ui(r, r, 2);
        mpz_mod_ui(temp, r, 2);
    }
    mpz_sub_ui(s_minus_one, s, 1);
    for (uint64_t i = 1; i < iters; i += 1) {
        mpz_sub_ui(temp, n, 3);
        //Reason for subtracting 3 from n is the range of mpz_urandomm is (0, n - 1) inclusive
        mpz_urandomm(a, state, temp);
        //Generates a random number from range (0, n - 4) inclusive
        //Adding 2 to the generated random number shifts the range to (2, n - 2) inclusive
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_minus_one) != 0) {
            mpz_set_ui(j, 1);
            while (mpz_cmp(j, s_minus_one) <= 0 && mpz_cmp(y, n_minus_one) != 0) {
                pow_mod(y, y, exponent, n);
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, n_minus_one) != 0) {
                mpz_clears(a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent, NULL);
                return false;
            }
        }
    }
    mpz_clears(a, s, r, y, j, temp, n_minus_one, s_minus_one, exponent, NULL);
    return true;
}
//Generates a new prime number stored in p
//The generated prime should be at least bits number of bits long
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    int base = 2;
    //Check to see if the generated prime matches the conditions
    //Conditions are the number is prime and is at least bits number of bits long
    while (!is_prime(p, iters) || mpz_sizeinbase(p, base) < bits) {
        mpz_urandomb(p, state, bits);
    }
    return;
}
