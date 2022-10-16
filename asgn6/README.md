# Assignment 6 - Public Key Cryptography

This assignment implements public key cryptography, a system that uses pairs of keys: public keys and private keys to encrypt and decrypt messages. For this assignment, there will be three programs: keygen, encrypt, and decrypt. Keygen will produce the RSA public and private key pairs, encrypt will encrypt files using the public key, and decrypt will decrypt files using the corresponding private key. All three programs will have help and verbose printing command-line options which are specificed by h and v respectively. Keygen will take in command-line arguments b, for the minimum bits the public key n can have, n, specifying the public key file, and d, specifying the private key file. Encrypt will take in command-line arguments i, for the input file to encrypt, o, for the output file to write the encrypted contents to, and n, for the public key file. Decrypt will take in command-line arguments i, for the input file to decrypt, o, for the output file to write the decrypted contents to, and n, for the private key file.

## Building

Build the program(s) with:

$ make keygen

or 

$ make encrypt

or

$ make decrypt

or  

$ make all

## Running

Run the program(s) with:

$ ./keygen [-hv] [-b bits] -n pbfile -d pvfile 

or

$ ./encrypt [-hv] [-i infile] [-o outfile] -n pubkey

or 

$ ./decrypt [-hv] [-i infile] [-o outfile] -n privkey

