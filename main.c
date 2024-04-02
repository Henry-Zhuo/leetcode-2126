#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>



enum RETURN_CODES {
    SUCCESS = 0,
    FAIL_MEMORY_ALLOC_ERROR = -1,
    INCORRECT_ARG_COUNT_ERROR = -2,
    NOT_ENOUGH_ASTEROIDS_ERROR = -3,
};



/* Solution for problem Leetcode 2126.
    This is similar to quicksort, but we absorb asteroids in the left subarray
    and recurse only on the second subarray. The absorbing happens immediately,
    so there is no left subarray to keep track of.

    Note that negative asteroid masses aren't filtered out and would decrease
    the planet's mass, possibly leading to a reported failure, when we could
    absorb positive masses first and possibly succeed.
    Ex: mass = 10, asteroids = [-10, 5]. We can absorb 5 first to get 15, then
    -10 to get 5. If we absorb -10 first, we have 0 mass and can't absorb the 5.

    TODO: Benchmark if it is faster to absorb asteroids after the first loop
    ends, using a dedicated for loop since that can be vectorized.
*/
bool asteroidsDestroyed(long long int mass, int* asteroids, int asteroidsSize) {
    bool swapped;
    int rightSubarrayStartIdx;
    int scanRightSideIdx = asteroidsSize - 1;

    for (rightSubarrayStartIdx = 0; rightSubarrayStartIdx < asteroidsSize; rightSubarrayStartIdx++) {
        if (mass >= asteroids[rightSubarrayStartIdx]) {
            // Can absorb asteroid
            mass += asteroids[rightSubarrayStartIdx];
        } else {
            swapped = false;

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
    count, the asteroid masses given as a comma seperated sequence, then the
    expected result.
    Ex: `a 10 6 2,7,12,33,10,6 0` is testing planet mass of 10, and 6 asteroids
    with mass 2, 7, 12, 33, 10, and 6, and an expected result of 0. `a` is the
    name of the compiled program. Extra asteroids in the sequence are ignored
    and give a warning. The last number in sequences shouldn't have a comma
    after it.
*/
int main(int argc, char *argv[]) {
    const int EXPECTED_ARGUMENT_COUNT = 4;
    const int PLANET_MASS_IDX = 1;
    const int ASTEROID_COUNT_IDX = 2;
    const int ASTEROIDS_MASS_SEQUENCE_IDX = 3;
    const int EXPECTED_RESULT_IDX = 4;
    char *asteroidsSequence;
    int mass;
    int *asteroids;
    int asteroidsSize;
    bool result;
    int return_code;

    // argc counts the program name as an argument, subtract 1 to get argument
    // count.
    if (argc - 1 != EXPECTED_ARGUMENT_COUNT) {
        fprintf(
            stderr,
            "ERROR: Expected %d arguments, got %d.\n",
            EXPECTED_ARGUMENT_COUNT,
            argc - 1
        );
        return INCORRECT_ARG_COUNT_ERROR;
    }

    asteroidsSize = atoi(argv[ASTEROID_COUNT_IDX]);
    asteroids = malloc(sizeof(int) * asteroidsSize);
    if (asteroids == NULL) {
        fprintf(
            stderr,
            "ERROR: Cannot allocate memory to store %d asteroids\n",
            asteroidsSize
        );
        return FAIL_MEMORY_ALLOC_ERROR;
    }

    asteroidsSequence = argv[ASTEROIDS_MASS_SEQUENCE_IDX];
    int lastCommaIdx = -1;
    int asteroidsProcessed = 0;
    for (int i = 0; asteroidsSequence[i] != '\0'; i++) {
        if (asteroidsSequence[i] == ',') {
            // Mutating input string since we won't be reusing it.
            // We split comma separated numbers into their own string in place.
            asteroidsSequence[i] = '\0';
            asteroids[asteroidsProcessed] = atoi(asteroidsSequence + lastCommaIdx + 1);
            asteroidsProcessed++;
            lastCommaIdx = i;
        }
    }
    asteroids[asteroidsProcessed] = atoi(asteroidsSequence + lastCommaIdx + 1);
    asteroidsProcessed++;

    if (asteroidsProcessed < asteroidsSize) {
        fprintf(
            stderr,
            "ERROR: Not enough asteroids, was told there should be %d asteroids but only %d were given.\n",
            asteroidsSize,
            asteroidsProcessed
        );
        return_code = NOT_ENOUGH_ASTEROIDS_ERROR;
        goto cleanup;
    }
    if (asteroidsSize > asteroidsProcessed) {
        fprintf(
            stderr,
            "WARN: Was told there should be %d asteroids but %d were given.\n",
            asteroidsSize,
            asteroidsProcessed
        );
        asteroidsSize = asteroidsProcessed;
    }

    mass = atoi(argv[PLANET_MASS_IDX]);

    int expected_result = atoi(argv[EXPECTED_RESULT_IDX]);
    result = asteroidsDestroyed(mass, asteroids, asteroidsSize);
    // Ensure 0 is returned if result is as expected, 1 if not
    return_code = 1 - (result == expected_result) % expected_result;

    // Cleanup
cleanup:
    free(asteroids);

    return return_code;
}
