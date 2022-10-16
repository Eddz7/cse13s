#include "insert.h"

#include "stats.h"

#include <inttypes.h>
#include <stdio.h>

void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    int j;
    int temp;
    for (uint32_t i = 1; i < n; i += 1) {
        j = i;
        temp = move(stats, A[i]);
        while (j > 0 && cmp(stats, temp, A[j - 1]) == -1) {
            A[j] = move(stats, A[j - 1]);
            j -= 1;
        }
        A[j] = move(stats, temp);
    }
}
