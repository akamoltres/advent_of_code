#pragma once

// Returns length of intcode program if success
// Returns -1 if file is invalid
// Returns -2 if buffer is not long enough
int read_intcode(const int bufsize, int *buffer, char *filename);

// Returns 0 if program terminated successfully (opcode 99)
// Returns -1 if program terminated unexpectedly
int run_intcode(const int program_length, int *buffer);
