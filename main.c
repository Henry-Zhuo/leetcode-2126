#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



enum ERROR_CODES {
    FAIL_MEMORY_ALLOC_ERROR = -1,
    INCORRECT_ARG_COUNT_ERROR = -2,
};



/* Submission for problem Leetcode 2126.
    This is similar to quicksort, but we absorb asteroids in the left subarray
    and recurse only on the second subarray. The absorbing happens immediately,
    so there is no left subarray to keep track of.

    TODO: Benchmark if it is faster to absorb asteroids after the first loop
    ends, using a dedicated for loop since that can be vectorized.
*/
bool asteroidsDestroyed(int mass, int* asteroids, int asteroidsSize) {
    int rightSubarrayStartIdx;
    int scanRightSideIdx = asteroidsSize - 1;

    for (rightSubarrayStartIdx = 0; rightSubarrayStartIdx < asteroidsSize; rightSubarrayStartIdx++) {
        if (mass >= asteroids[rightSubarrayStartIdx]) {
            // Can absorb asteroid
            mass += asteroids[rightSubarrayStartIdx];
        } else {
            bool swapped = false;

            for ( ; scanRightSideIdx > rightSubarrayStartIdx; scanRightSideIdx--) {
                if (mass >= asteroids[scanRightSideIdx]) {
                    mass += asteroids[scanRightSideIdx];
                    asteroids[scanRightSideIdx] = asteroids[rightSubarrayStartIdx];
                    swapped = true;
                    break;
                }
            }

            if (swapped == false) {
                // Searched from the right side, didn't find anything to swap,
                // therefore all elements on the right are larger than mass
                break;
            }
        }
    }

    if (rightSubarrayStartIdx == asteroidsSize) {
        // No asteroids have more mass than planet, can absorb all asteroids
        return true;
    } else if (rightSubarrayStartIdx == 0) {
        // Can't absorb asteroids on the right and no asteroids to increase
        // mass.
        return false;
    }

    // Recurse on right subarray which has asteroids[rightSubarrayStartIdx] as
    // its 0th element.
    return asteroidsDestroyed(
        mass,
        asteroids + rightSubarrayStartIdx,
        asteroidsSize - rightSubarrayStartIdx
    );
}



/* Testing and commandline interface for asteroidsDestroyed()

    When using commandline arguments, we expect the planet mass, the asteroid
    count, then the asteroid masses given as a comma seperated sequence.
    Ex: `a 10 6 2,7,12,33,10,6` is testing planet mass of 10, and 6 asteroids
    with mass 2, 7, 12, 33, 10, and 6. `a` is the name of the compiled
    program. Extra asteroids in the sequence are ignored and give a warning.
*/
int main(int argc, char *argv[]) {
    const int EXPECTED_ARGUMENT_COUNT = 3;
    const int PLANET_MASS_IDX = 1;
    const int ASTEROID_COUNT_IDX = 2;
    const int ASTEROIDS_MASS_SEQUENCE_IDX = 3;
    char *asteroidsSequence;
    int mass;
    int *asteroids;
    int asteroidsSize;
    bool result;

    // argc counts the program name as an argument, subtract 1 to get argument
    // count.
    if (argc - 1 != EXPECTED_ARGUMENT_COUNT) {
        fprintf(
            stderr,
            "ERROR: Expected %d arguments, got %d.",
            EXPECTED_ARGUMENT_COUNT,
            argc
        );
        return INCORRECT_ARG_COUNT_ERROR;
    }

    asteroidsSize = atoi(argv[ASTEROID_COUNT_IDX]);
    asteroids = malloc(sizeof(int) * asteroidsSize);
    if (asteroids == NULL) {
        fprintf(
            stderr,
            "ERROR: Cannot allocate memory to store %d asteroids",
            asteroidsSize
        );
        return FAIL_MEMORY_ALLOC_ERROR;
    }

    asteroidsSequence = argv[ASTEROIDS_MASS_SEQUENCE_IDX];
    int lastCommaIdx = -1;
    int asteroidsProcessed = 0;
    for (int i = 0; asteroidsSequence[i] != '\n'; i++) {
        if (asteroidsSequence[i] == ',') {
            // Mutating input string since we won't be reusing it.
            // We split comma separated numbers into their own string in place.
            asteroidsSequence[i] = '\0';
            asteroids[asteroidsProcessed] = atoi(asteroidsSequence + lastCommaIdx + 1);
            lastCommaIdx = i;
            asteroidsProcessed++;
        }
    }
    if (asteroidsSize > asteroidsProcessed) {
        fprintf(
            stderr,
            "WARN: Was told there should be %d asteroids but only %d were given.",
            asteroidsProcessed,
            asteroidsSize
        );
        asteroidsSize = asteroidsProcessed;
    }

    mass = atoi(argv[PLANET_MASS_IDX]);

    result = asteroidsDestroyed(mass, asteroids, asteroidsSize);

    // Cleanup
    free(asteroids);

    return result;
}