# Assignment 7 - The Great Firewall of Santa Cruz

This purpose of this assignment is to implement a bloom filter and a hash table in order to filter out oldspeak, or words you deem improper as the one and only leader of the Glorious People's Republic of Santa Cruz (GPRSC). For this assignment a program, banhammer, will be implemented to filter out oldspeak words using a bloom filter and a hash table. A bloom filter is a probabilistic data structure that is used to test whether an element is a member of a set or not. The bloom filter takes in a list of proscribed words called oldspeak and checks if any citizens are using oldspeak depending if any of the words used were added to the bloom filter. A hash table is a data structure that maps keys to values and is used to store translations from oldspeak to newspeak as well as oldspeak words without newspeak translations. A hash table will determine which citizens need counseling for using oldspeak with newspeak translations and which citizens will be punished for using badspeak, or oldspeak words without newspeak translations. The program banhammer will have command-line options h and s which will print the help message for the program and statistics respectively. The program will also have command-line arguments t, the size of hash table, and f, the size of the bloom filter.

## Building

Build the program with:

$ make all

or

$ make banhammer

## Running

Run the program with:

$ ./banhammer [-hs] [-t size] [-f size]

