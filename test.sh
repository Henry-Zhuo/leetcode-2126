#! /bin/bash

# Testing script for problem Leetcode 2126 Destroying Asteroids

EXECUTABLE="build/exe"

# Test cases are commandline arguments
TEST_CASES=(
    "10 8 1,2,3,4,5,6,7,8 0"
)



success_count=0
fail_count=0

for i in "${!TEST_CASES[@]}"; do
    if [[ $(./${EXECUTABLE} ${TEST_CASES[i]}) = 0 ]]; then
        echo "Failed test case ${i}"
        fail_count=$((fail_count + 1))
    else
        success_count=$((success_count + 1))
    fi
done

echo "Passed test cases: ${success_count}"
echo "Failed test cases: ${fail_count}"
