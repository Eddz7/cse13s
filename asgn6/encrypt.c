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
    int base = 62;
    FILE *input;
    FILE *output;
    FILE *public_key;
    char *in_file = NULL;
    char *out_file = NULL;
    char *pub_file = "rsa.pub";
    char user_name[256];
    bool test_v, test_h;
    test_v = test_h = false;
    mpz_t username, n, e, s;
    mpz_inits(username, n, e, s, NULL);
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
               "   Encrypts data using RSA encryption.\n"
               "   Encrypted data is decrypted by the decrypt program.\n\n"
               "USAGE\n"
               "   ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey\n\n"
               "OPTIONS\n"
               "   -h\t\t   Display program help and usage.\n"
               "   -v\t\t   Display verbose program output.\n"
               "   -i infile\t   Input file of data to encrypt (default: stdin).\n"
               "   -o outfile\t   Output file for encrypted data (default: stdout).\n"
               "   -n pbfile\t   Public key file (default: rsa.pub).\n");
        return 0;
    }

    //Checks if the file chars for input and output were changed
    //If changed opens files with file names normally
    //If not changed, sets the input file to stdin and output file to stdout
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

    public_key = fopen(pub_file, "r");
    rsa_read_pub(n, e, s, user_name, public_key);
    if (test_v) {
        printf("user = %s\n", user_name);
        gmp_printf("s (%zu bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("n (%zu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e (%zu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    }
    mpz_set_str(username, user_name, base);
    //RSA verification: print an error and end the program if username could not be verified
    if (!rsa_verify(username, s, e, n)) {
        printf("Error: could not verify user.\n");
        return 0;
    }
    rsa_encrypt_file(input, output, n, e);
    fclose(public_key);
    fclose(input);
    fclose(output);
    mpz_clears(username, n, e, s, NULL);
    return 0;
}
