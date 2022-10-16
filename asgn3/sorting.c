#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS          "haeisqn:p:r:"
#define DEFAULT_SEED     13371453
#define DEFAULT_SIZE     100
#define DEFAULT_ELEMENTS 100

typedef enum { INSERTION, HEAP, SHELL, QUICK } Sorts;
//test harness for sorts, the random seed, size of array, and num of elements to print
int main(int argc, char **argv) {

    Set s = empty_set();
    const unsigned mask = (1 << 30) - 1;
    long rand = DEFAULT_SEED;
    char *rand_ptr;
    long size = DEFAULT_SIZE;
    char *size_ptr;
    uint32_t *A;
    long elem = DEFAULT_ELEMENTS;
    char *elem_ptr;
    long prnt_elements;
    bool h_test = false;

    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {

        case 'e': s = insert_set(HEAP, s); break;

        case 's': s = insert_set(SHELL, s); break;

        case 'i': s = insert_set(INSERTION, s); break;

        case 'q': s = insert_set(QUICK, s); break;

        case 'a':
            s = insert_set(HEAP, s);
            s = insert_set(SHELL, s);
            s = insert_set(INSERTION, s);
            s = insert_set(QUICK, s);
            break;

        case 'r': rand = strtoul(optarg, &rand_ptr, 10); break;

        case 'n': size = strtoul(optarg, &size_ptr, 10); break;

        case 'p': elem = strtoul(optarg, &elem_ptr, 10); break;

        case 'h': h_test = true;
        }
    }

    if (argc == 1 || h_test) {
        printf("SYNOPSIS\n"
               "   A collection of comparison-based sorting algorithms.\n\n"
               "USAGE\n"
               "   ./sorting [-haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\n"
               "OPTIONS\n"
               "   -h\t\t   display program help and usage.\n"
               "   -a\t\t   enable all sorts.\n"
               "   -e\t\t   enable Heap Sort.\n"
               "   -i\t\t   enable Insertion Sort.\n"
               "   -s\t\t   enable Shell Sort.\n"
               "   -q\t\t   enable Quick Sort.\n"
               "   -n length\t   specify number of array elements (default: 100).\n"
               "   -p elements\t   specify number of elements to print (default: 100).\n"
               "   -r seed\t   specify random seed (default: 13371453).\n");
        return 0;
    }

    Stats stats;
    reset(&stats);

    //Sizing array here after size has been determined
    A = (uint32_t *) malloc(size * sizeof(uint32_t));

    //Setting random seed in srandom after rand has been determined
    srandom(rand);
    for (uint32_t i = 0; i < size; i += 1) {
        A[i] = (random() & mask);
    }
    //Sets the number of array elements to print
    //Makes it so there isn't two for loops for printing elements in each sort
    if (size < elem) {
        prnt_elements = size;
    } else {
        prnt_elements = elem;
    }

    if (member_set(HEAP, s)) {
        reset(&stats);
        heap_sort(&stats, A, size);
        printf("Heap Sort, %ld elements, ", size);
        printf("%lu moves, ", stats.moves);
        printf("%lu compares\n", stats.compares);
        for (int i = 0; i < prnt_elements; i += 1) {
            printf("%13" PRIu32, A[i]);
            //Every fifth element printed prints a new line
            //A new line is also printed once the last element is printed
            if ((i + 1) % 5 == 0 && (i + 1) >= 5) {
                printf("\n");
            } else if (i == prnt_elements - 1) {
                printf("\n");
            }
        }
        //Refill array A with the same random numbers
        srandom(rand);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = (random() & mask);
        }
    }

    if (member_set(SHELL, s)) {
        reset(&stats);
        shell_sort(&stats, A, size);
        printf("Shell Sort, %ld elements, ", size);
        printf("%lu moves, ", stats.moves);
        printf("%lu compares\n", stats.compares);
        for (int i = 0; i < prnt_elements; i += 1) {
            printf("%13" PRIu32, A[i]);
            if ((i + 1) % 5 == 0 && (i + 1) >= 5) {
                printf("\n");
            } else if (i == prnt_elements - 1) {
                printf("\n");
            }
        }
        srandom(rand);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = (random() & mask);
        }
    }

    if (member_set(INSERTION, s)) {
        reset(&stats);
        insertion_sort(&stats, A, size);
        printf("Insertion Sort, %ld elements, ", size);
        printf("%lu moves, ", stats.moves);
        printf("%lu compares\n", stats.compares);
        for (int i = 0; i < prnt_elements; i += 1) {
            printf("%13" PRIu32, A[i]);
            if ((i + 1) % 5 == 0 && (i + 1) >= 5) {
                printf("\n");
            } else if (i == prnt_elements - 1) {
                printf("\n");
            }
        }
        srandom(rand);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = (random() & mask);
        }
    }

    if (member_set(QUICK, s)) {
        reset(&stats);
        quick_sort(&stats, A, size);
        printf("Quick Sort, %ld elements, ", size);
        printf("%lu moves, ", stats.moves);
        printf("%lu compares\n", stats.compares);
        for (int i = 0; i < prnt_elements; i += 1) {
            printf("%13" PRIu32, A[i]);
            if ((i + 1) % 5 == 0 && (i + 1) >= 5) {
                printf("\n");
            } else if (i == prnt_elements - 1) {
                printf("\n");
            }
        }
        srandom(rand);
        for (uint32_t i = 0; i < size; i += 1) {
            A[i] = (random() & mask);
        }
    }
    //Free memory from malloc()
    free(A);
}
