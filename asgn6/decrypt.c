#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

#define OPTIONS "i:o:n:vh"
int main(int argc, char **argv) {
    FILE *input;
    FILE *output;
    FILE *private_key;
    char *in_file = NULL;
    char *out_file = NULL;
    char *pub_file = "rsa.priv";
    bool test_v, test_h;
    test_v = test_h = false;
    mpz_t n, d;
    mpz_inits(n, d, NULL);
    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': in_file = optarg; break;
        case 'o': out_file = optarg; break;
        case 'n': pub_file = optarg; break;
        case 'v': test_v = true; break;
        case 'h': test_h = true; break;
        }
    }
    if (test_h) {
        printf("SYNOPSIS\n"
               "   Decrypts data using RSA decryption.\n"
               "   Encrypted data is encrypted by the encrypt program.\n\n"
               "USAGE\n"
               "   ./decrypt [-hv] [-i infile] [-o outfile] -n privkey\n\n"
               "OPTIONS\n"
               "   -h\t\t   Display program help and usage.\n"
               "   -v\t\t   Display verbose program output.\n"
               "   -i infile\t   Input file of data to decrypt (default: stdin).\n"
               "   -o outfile\t   Output file for decrypted data (default: stdout).\n"
               "   -n pvfile\t   Private key file (default: rsa.priv).\n");
        return 0;
    }

    //Checks if the file chars for input and output were changed
    //If changed open the input file for reading and output file for writing with char names
    //If not changed then set the input and output to stdin and stdout respectively
    if (in_file != NULL) {
        input = fopen(in_file, "r");
    } else {
        input = stdin;
    }

    if (out_file != NULL) {
        output = fopen(out_file, "w");
    } else {
        output = stdout;
    }
    private_key = fopen(pub_file, "r");
    rsa_read_priv(n, d, private_key);
    if (test_v) {
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("d (%zu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    rsa_decrypt_file(input, output, n, d);
    fclose(private_key);
    fclose(input);
    fclose(output);
    mpz_clears(n, d, NULL);
    return 0;
}
