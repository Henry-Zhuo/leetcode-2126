#! /bin/bash

# Testing script for problem Leetcode 2126 Destroying Asteroids
# An improved setup would report the label for the failed tests.
# There isn't a need for checking of return codes, such as error codes, for now.

EXECUTABLE="build/exe"

# Test cases are commandline arguments
# TODO: Test bad input. Need to check return status of program
TEST_CASES=(
    # 1. Can absorb starting with last asteroid then work your way from the left
    "10 6 20,30,40,50,60,10 1"
    # 2. Can't absorb 20 after absorbing 6 and 1 to reach 17
    "10 6 20,30,40,50,6,1 0"
    # 3. Can absorb all asteroids, planet mass is roughly the average asteroid mass
    "30 60 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60 1"
    # 4. Can't absorb last asteroid
    "30 60 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,1000000 0"
    # 5. Can't absorb any asteroids
    "10 60 11,12,13,14,15,16,17,18,19,20,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60 0"
    # No asteroids to absorb
    "10 0 1 1"
    # Can absorb, all asteroids are smaller than planet
    "20 10 1,2,3,4,5,6,7,8,9,10 1"
    # Can't absorb single asteroid
    "10 1 200 0"
    # Can absorb single asteroid
    "10 1 2 1"

    # Bad input tests
    # Not enough arguments
    "10"
    "10 1"
    "10 1 4"
    # Not enough asteroids
    "10 10 1,2 -3"
)



success_count=0
fail_count=0

for i in "${!TEST_CASES[@]}"; do
    if [[ $(./${EXECUTABLE} ${TEST_CASES[i]}) -ne 0 ]]; then
        echo "!! Failed test case ${i} !!"
        fail_count=$((fail_count + 1))
    else
        success_count=$((success_count + 1))
    fi
done

echo "Passed test cases: ${success_count}"
echo "Failed test cases: ${fail_count}"

if [[ $fail_count = 0 ]]; then
    echo "== All tests passed! =="
else
    echo "!! Failure detected! Check above for which errors failed !!"
fi
