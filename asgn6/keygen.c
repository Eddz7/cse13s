#include <stdbool.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "randstate.h"
#include "rsa.h"

#define OPTIONS "b:i:n:d:s:vh"
int main(int argc, char **argv) {
    char *seed_ptr, *iters_ptr, *bits_ptr;
    char *user_name = NULL;
    uint64_t seed = time(NULL);
    uint64_t iters = 50;
    uint64_t modulus = 256;
    //Need to make char variables for pub and priv key so they can be changed by optarg
    char *pub_key = "rsa.pub";
    char *priv_key = "rsa.priv";
    bool test_v, test_h;
    test_v = test_h = false;
    int base = 62;
    //Specifies the base for converting the string username into an mpz_t
    mpz_t name, p, q, n, e, d, s;
    mpz_inits(name, p, q, n, e, d, s, NULL);
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': modulus = strtoul(optarg, &bits_ptr, 10); break;
        case 'i': iters = strtoul(optarg, &iters_ptr, 10); break;
        case 'n': pub_key = optarg; break;
        case 'd': priv_key = optarg; break;
        case 's': seed = strtoul(optarg, &seed_ptr, 10); break;
        case 'v': test_v = true; break;
        case 'h': test_h = true; break;
        }
    }

    if (test_h) {
        printf("SYNOPSIS\n"
               "   Generates an RSA public/private key pair.\n\n"
               "USAGE\n"
               "   ./keygen [-hv] [-b bits] -n pbfile -d pvfile\n\n"
               "OPTIONS\n"
               "   -h\t\t   Display program help and usage.\n"
               "   -v\t\t   Display verbose program output.\n"
               "   -b bits\t   Minimum bits needed for public key n (default: 256).\n"
               "   -i confidence   Miller-Rabin iterations for testing primes (default: 50).\n"
               "   -n pbfile\t   Public key file (default: rsa.pub).\n"
               "   -d pvfile\t   Private key file (default: rsa.priv).\n"
               "   -s seed\t   Random seed for testing.\n");
        return 0;
    }

    FILE *public_key, *private_key;
    public_key = fopen(pub_key, "w");
    private_key = fopen(priv_key, "w");
    fchmod(fileno(private_key), 0600);
    //Sets the private key permissions of read and write for the user only
    randstate_init(seed);
    rsa_make_pub(p, q, n, e, modulus, iters);
    rsa_make_priv(d, e, p, q);
    user_name = getenv("USER");
    mpz_set_str(name, user_name, base);
    //Produces signature s by signing message m (user_name)
    rsa_sign(s, name, d, n);
    rsa_write_pub(n, e, s, user_name, public_key);
    rsa_write_priv(n, d, private_key);
    fclose(public_key);
    fclose(private_key);
    randstate_clear();

    if (test_v) {
        printf("user = %s\n", user_name);
        gmp_printf("s (%zu bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("p (%zu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%zu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    mpz_clears(name, p, q, n, e, d, s, NULL);
    return 0;
}
