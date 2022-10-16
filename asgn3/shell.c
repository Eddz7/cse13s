#include "shell.h"

#include "stats.h"

#include <math.h>
#include <stdio.h>

void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    int gap;
    int j;
    int temp;
    for (int i = (log(3 + 2 * n) / log(3)); i > 0; i -= 1) {
        gap = ((pow(3, i) - 1) / 2);
        for (uint32_t ii = gap; ii < n; ii += 1) {
            j = ii;
            temp = move(stats, A[ii]);
            while (j >= gap && cmp(stats, temp, A[j - gap]) == -1) {
                A[j] = move(stats, A[j - gap]);
                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    }
}
