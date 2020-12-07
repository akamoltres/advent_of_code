CC=gcc
CFLAGS=-Iinclude/

BUILD_DIR=build
SRC_DIR=src
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
OBJ_FILES=$(patsubst %.c,%.o,$(SRC_FILES))
EXE_FILES=$(wildcard $(BUILD_DIR)/[0-9].o)
EXE_FILES+=$(wildcard $(BUILD_DIR)/[0-9][0-9].o)
EXE_FILES:=$(patsubst %.o,%.out,$(EXE_FILES))
LIB_FILES=$(find ./build -not -regex "./build/[0-9]+\.o")
#LIB_FILES=$(OBJ_FILES)
#LIB_EXE=$(patsubst %.out,%.o,$(EXE_FILES))
#LIB_FILES-=$(LIB_EXE)

.PHONY: all
all: build_dir $(OBJ_FILES)
#	@echo $(EXE_FILES)
	@echo $(LIB_FILES)

.PHONY: clean
clean:
	@rm -r $(BUILD_DIR)

%.o: %.c
	@$(CC) -c -o $(BUILD_DIR)/$(notdir $@) $^ $(CFLAGS)

build_dir:
	@mkdir -p $(BUILD_DIR)
