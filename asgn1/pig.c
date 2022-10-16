#include "names.h"

#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define MIN_PLAYERS      2
#define MAX_PLAYERS      10
#define SEED_MIN         0
#define SEED_MAX         UINT_MAX
#define SIDE_POINTS      0
#define RAZORBACK_POINTS 10
#define TROTTER_POINTS   10
#define SNOUTER_POINTS   15
#define JOWLER_POINTS    5
typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;
const Position pig[7] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

int points[10] = { 0 };

// Contains implementation of the game Pass the Pigs
// Main takes no arguments
// Returns 0 if the program exectued with no errors
int main(void) {
    uint64_t rand_seed;
    int input;
    int roll;

    printf("How many players? ");
    scanf("%d", &input);
    if (input < MIN_PLAYERS || input > MAX_PLAYERS) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        input = MIN_PLAYERS;
    }
    printf("Random seed: ");
    scanf("%lu", &rand_seed);
    if (SEED_MIN <= rand_seed && rand_seed <= SEED_MAX) {
        srandom(rand_seed);
    } else {
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        rand_seed = 2021;
        srandom(rand_seed);
    }

    for (int i = 0; i < input; i += 1) {
        printf("%s", names[i]);
        printf(" rolls the pig...");

        while (points[i] < 100) {
            roll = pig[random() % 7];
            if (roll == SIDE) {
                printf(" pig lands on side");
                points[i] += SIDE_POINTS;
                break;
            } else if (roll == RAZORBACK) {
                printf(" pig lands on back");
                points[i] += RAZORBACK_POINTS;
            } else if (roll == TROTTER) {
                printf(" pig lands upright");
                points[i] += TROTTER_POINTS;
            } else if (roll == SNOUTER) {
                printf(" pig lands on snout");
                points[i] += SNOUTER_POINTS;
            } else if (roll == JOWLER) {
                printf(" pig lands on ear");
                points[i] += JOWLER_POINTS;
            }
        }

        printf("\n");

        if (i == input - 1 && points[i] < 100) {
            i = -1;
            continue;
        }

        if (points[i] >= 100) {
            printf("%s", names[i]);
            printf(" wins with ");
            printf("%d", points[i]);
            printf(" points!\n");
            break;
        }
    }

    return 0;
}
