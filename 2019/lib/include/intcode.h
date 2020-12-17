#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct IntcodeReturn
{
    int pc;
    int input_used;
    int retval;
    int halt;
} IntcodeReturn_t;

// Prints the contents of the intcode program buffer
void print_program(int program_length, int *buffer);

// Returns length of intcode program if success
// Returns -1 if file is invalid
// Returns -2 if buffer is not long enough
int read_intcode(const int bufsize, int *buffer, const char *filename);

// Returns 0 if program terminated successfully (opcode 99)
// Returns -1 if program terminated unexpectedly
IntcodeReturn_t run_intcode(const int program_length, const int bufsize, int *buffer, const int input_length, int *input_buffer, int pc);

#ifdef __cplusplus
}
#endif
