CC=gcc
CXX=g++
CFLAGS=-Ilib/include/ -Isol/include/
CPPFLAGS=-I/usr/include/CppUTest/
LD_LIBRARIES=-L/usr/lib/x86_64-linux-gnu/ -lCppUTest
DEPS=$(wildcard lib/include) $(wildcard sol/include)
#CPPUTEST_HOME=

BUILD_DIR=build
SOL_SRC_FILES=$(wildcard sol/src/*.c)
SOL_OBJ_FILES=$(patsubst %.c,%.o,$(SOL_SRC_FILES))
SOL_EXE_FILES=$(patsubst %.o,%.out,$(SOL_OBJ_FILES))
LIB_SRC_FILES=$(wildcard lib/src/*.c)
LIB_OBJ_FILES=$(patsubst %.c,%.o,$(LIB_SRC_FILES))
TEST_SRC_FILES=$(wildcard test/*.cpp)
TEST_OBJ_FILES=$(patsubst %.cpp,%.o,$(TEST_SRC_FILES))

.PHONY: all
all: build_dir $(SOL_EXE_FILES)

.PHONY: test
test: build_dir $(TEST_OBJ_FILES) $(SOL_OBJ_FILES)
	@$(CXX) -o $(BUILD_DIR)/tests.out $(addprefix $(BUILD_DIR)/,$(notdir $(TEST_OBJ_FILES))) $(CPPFLAGS) $(LD_LIBRARIES)

.PHONY: clean
clean:
	@rm -r $(BUILD_DIR)

%.out: %.o $(LIB_OBJ_FILES)
	@$(CC) -o $(BUILD_DIR)/$(notdir $@) $(addprefix $(BUILD_DIR)/,$(notdir $^)) $(CFLAGS)

%.o: %.c $(DEPS)
	@$(CC) -c -o $(BUILD_DIR)/$(notdir $@) $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	@$(CC) -c -o $(BUILD_DIR)/$(notdir $@) $< $(CPPFLAGS)

build_dir:
	@mkdir -p $(BUILD_DIR)
