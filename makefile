BUILD_DIR := build
EXECUTABLE := exe


all : *.c
	mkdir -p $(BUILD_DIR)
	gcc -o $(BUILD_DIR)/$(EXECUTABLE) *.c



# Make sure test.sh is executable
test : all
	./test.sh
