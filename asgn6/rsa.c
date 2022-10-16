#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"
#include <stdlib.h>
//Creates parts of a RSA key: two large primes p and q, their product n, and the exponent e
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    mpz_t q_bits, p_bits, q_minus_one, p_minus_one, totient, public_exponent, gcd_temp;
    mpz_inits(q_bits, p_bits, q_minus_one, p_minus_one, totient, public_exponent, gcd_temp, NULL);
    uint64_t p_rand = (random() % 3);
    //This generates a random number in the range [0, 2] inclusive
    p_rand += 1;
    p_rand *= (nbits / 4);
    //add 1 to (0, 2) to get range of (1, 3) then multiply by nbits/4
    //this gets a random number in the range of (nbits/4, (3*nbits)/4)
    mpz_set_ui(p_bits, p_rand);
    mpz_set_ui(q_bits, (nbits - p_rand));
    make_prime(p, (p_rand + 1), iters);
    make_prime(q, (nbits - p_rand + 1), iters);
    mpz_mul(n, q, p);
    mpz_sub_ui(q_minus_one, q, 1);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_mul(totient, q_minus_one, p_minus_one);
    mpz_urandomb(public_exponent, state, nbits);
    gcd(gcd_temp, public_exponent, totient);
    while (mpz_cmp_ui(gcd_temp, 1) != 0) {
        mpz_urandomb(public_exponent, state, nbits);
        gcd(gcd_temp, public_exponent, totient);
    }
    mpz_set(e, public_exponent);
    mpz_clears(q_bits, p_bits, q_minus_one, p_minus_one, totient, public_exponent, gcd_temp, NULL);
    return;
}
//Writes a public RSA key to pbfile
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
    return;
}
//Reads a public RSA key from pbfile
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
    return;
}
//Creates a private RSA key d given primes p and q and the public exponent e
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t totient, q_minus_one, p_minus_one;
    mpz_inits(totient, q_minus_one, p_minus_one, NULL);
    mpz_sub_ui(p_minus_one, p, 1);
    mpz_sub_ui(q_minus_one, q, 1);
    mpz_mul(totient, p_minus_one, q_minus_one);
    mod_inverse(d, e, totient);
    mpz_clears(totient, q_minus_one, p_minus_one, NULL);
    return;
}
//Writes a private RSA key to pvfile
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    return;
}
//Reads a private RSA key from pvfile
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
    return;
}
//Computes ciphertext c by encrypting message m using public exponent e and modulus n
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
    return;
}
//Encrypts the contents of infile and writes the encrypted contents to outfile
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    int base = 2;
    uint8_t *byte_array;
    size_t j;
    mpz_t n_temp, m, c;
    mpz_inits(n_temp, m, c, NULL);
    mpz_set(n_temp, n);
    size_t k = 0;
    k = mpz_sizeinbase(n, base);
    //Gets the floor of log base 2 n
    k = ((k - 1) / 8);
    byte_array = (uint8_t *) malloc(sizeof(uint8_t) * k);
    byte_array[0] = 0xFF;
    while (!feof(infile)) {
        j = fread(&byte_array[1], sizeof(uint8_t), (k - 1), infile);
        //Reads starting from byte_array[1] as not to overwrite the 0xFF
        if (j > 0) {
            mpz_import(m, (j + 1), 1, sizeof(uint8_t), 1, 0, byte_array);
            //(j + 1) in mpz_import includes the prepended 0xFF into m
            rsa_encrypt(c, m, e, n);
            gmp_fprintf(outfile, "%Zx\n", c);
        }
    }
    mpz_clears(n_temp, m, c, NULL);
    free(byte_array);
    return;
}
//Computes message m by decrypting ciphertext c using private key d and public modulus n
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    return;
}
//Decrypts the contents of infile and writes the decrypted contents to outfile
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    uint8_t *byte_array;
    int base = 2;
    size_t j;
    ;
    mpz_t n_temp, m, c;
    mpz_inits(n_temp, m, c, NULL);
    mpz_set(n_temp, n);
    size_t k = 0;
    int scanned = 0;
    k = mpz_sizeinbase(n, base);
    k = ((k - 1) / 8);
    byte_array = (uint8_t *) malloc(sizeof(uint8_t) * k);
    while (!feof(infile)) {
        scanned = gmp_fscanf(infile, "%Zx", c);
        if (scanned > 0) {
            //The variable scanned checks to see if there were any errors with reading infile
            //Decrypts the contents only when scanned > 0, meaning no errors with reading
            rsa_decrypt(m, c, d, n);
            j = 0;
            mpz_export(byte_array, &j, 1, sizeof(uint8_t), 1, 0, m);
            fwrite(byte_array + 1, sizeof(uint8_t), (j - 1), outfile);
            //The (j-1) in fwrite ensures that the prepended 0xFF is not included in output
        }
    }
    free(byte_array);
    mpz_clears(n_temp, m, c, NULL);
    return;
}
//Produces signature s by signing message m using private key d and public modulus n
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
    return;
}
//Performs RSA verification, returning true if the signature s is verified and false otherwise
//The signature is verified if and only if t (s^e(mod n)) is the same as the expected message m
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    } else {
        mpz_clear(t);
        return false;
    }
    mpz_clear(t);
}
