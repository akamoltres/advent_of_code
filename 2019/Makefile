CC=gcc
CFLAGS=-Ilib/include/ -Isol/include -g
DEPS=$(wildcard lib/include) $(wildcard sol/include)

BUILD_DIR=build
SOL_SRC_FILES=$(wildcard sol/src/*.c)
SOL_OBJ_FILES=$(patsubst %.c,%.o,$(SOL_SRC_FILES))
SOL_EXE_FILES=$(patsubst %.o,%.out,$(SOL_OBJ_FILES))
LIB_SRC_FILES=$(wildcard lib/src/*.c)
LIB_OBJ_FILES=$(patsubst %.c,%.o,$(LIB_SRC_FILES))

.PHONY: all
all: build_dir $(SOL_EXE_FILES)

.PHONY: clean
clean:
	@rm -r $(BUILD_DIR)

%.out: %.o $(LIB_OBJ_FILES)
	@$(CC) -o $(BUILD_DIR)/$(notdir $@) $(addprefix $(BUILD_DIR)/,$(notdir $^)) $(CFLAGS)

%.o: %.c $(DEPS)
	@$(CC) -c -o $(BUILD_DIR)/$(notdir $@) $< $(CFLAGS)

build_dir:
	@mkdir -p $(BUILD_DIR)
