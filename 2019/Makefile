CC=gcc
CXX=g++
CFLAGS=-Ilib/include/ -Isol/include/ -Wall -Werror
CPPFLAGS=-I/usr/include/CppUTest/ -Itest/ $(CFLAGS)
LD_LIBRARIES=-L/usr/lib/x86_64-linux-gnu/ -lCppUTest
DEPS=$(wildcard lib/include) $(wildcard sol/include)

BUILD_DIR=build
SOL_SRC_FILES=$(wildcard sol/src/*.c)
SOL_OBJ_FILES=$(patsubst %.c,%.o,$(SOL_SRC_FILES))
LIB_SRC_FILES=$(wildcard lib/src/*.c)
LIB_OBJ_FILES=$(patsubst %.c,%.o,$(LIB_SRC_FILES))
TEST_SRC_FILES=$(wildcard test/*.cpp)
TEST_OBJ_FILES=$(patsubst %.cpp,%.o,$(TEST_SRC_FILES))

.PHONY: all
all: build_dir $(SOL_OBJ_FILES)
# TODO write script to autogenerate driver

.PHONY: test
test: build_dir $(TEST_OBJ_FILES) $(SOL_OBJ_FILES) $(LIB_OBJ_FILES)
	@$(CXX) -o $(BUILD_DIR)/tests.out $(addprefix $(BUILD_DIR)/,$(notdir $(TEST_OBJ_FILES))) $(addprefix $(BUILD_DIR)/,$(notdir $(SOL_OBJ_FILES))) $(addprefix $(BUILD_DIR)/,$(notdir $(LIB_OBJ_FILES))) $(CPPFLAGS) $(LD_LIBRARIES)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR)

.PHONY: lint
lint:
	@find . -name *.h -o -name *.c -o -name *.hpp -o -name *.cpp | xargs clang-format-10 --style file --dry-run

.PHONY: lint-i
lint-i:
	@find . -name *.h -o -name *.c -o -name *.hpp -o -name *.cpp | xargs clang-format-10 --style file -i

%.o: %.c $(DEPS)
	@$(CC) -c -o $(BUILD_DIR)/$(notdir $@) $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	@$(CXX) -c -o $(BUILD_DIR)/$(notdir $@) $< $(CPPFLAGS)

build_dir:
	@mkdir -p $(BUILD_DIR)
