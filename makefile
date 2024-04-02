BUILD_DIR := build
DEBUG_DIR := debug
EXECUTABLE := exe



# TODO: Better test cases, and move testing to makefile? Testing all test cases
# 	with Valgrind may be useful, but there will be a lot of output.
valgrind : *.c
	mkdir -p $(DEBUG_DIR)
	gcc -g -o $(DEBUG_DIR)/$(EXECUTABLE) *.c
	valgrind --leak-check=full $(DEBUG_DIR)/$(EXECUTABLE) 10 8 1,2,3,4,5,6,7,8



all : *.c
	mkdir -p $(BUILD_DIR)
	gcc -o $(BUILD_DIR)/$(EXECUTABLE) *.c



# Make sure test.sh is executable
test : all
	./test.sh
