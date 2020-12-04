CC=gcc
CFLAGS=-Iinclude/

BUILD_DIR=build
SRC_DIR=src
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst %.c,%.o,$(SRC_FILES))

.PHONY: all
all: build_dir $(OBJ_FILES)

.PHONY: clean
clean:
	@rm -r $(BUILD_DIR)

%.o: %.c
	@$(CC) -c -o $(BUILD_DIR)/$(notdir $@) $^ $(CFLAGS)

build_dir:
	@mkdir -p $(BUILD_DIR)
