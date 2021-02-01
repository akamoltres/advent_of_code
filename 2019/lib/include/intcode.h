#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#define INTCODE_BUFFER_SIZE 5000

typedef struct IntcodeReturn
{
    int input_used;
    long retval;
    int halt;
} IntcodeReturn_t;

typedef struct Intcode
{
    long program[INTCODE_BUFFER_SIZE];
    int pc;
    long relative_base;
} Intcode_t;

// Prints the first [program_length] items in the intcode program buffer
void print_program(const int program_length, Intcode_t const *const program);

// Reads in an intcode program from the provided CSV file
// Initializes program counter and relative base of the intcode program to zero
// Returns the length of the intcode program if successful
int read_intcode(Intcode_t *program, char const *const filename);

// Runs an intcode program using the provided input until it produces an output,
// or until it halts.
IntcodeReturn_t run_intcode(Intcode_t *program, const int input_length, long *input_buffer);

#ifdef __cplusplus
}
#endif
