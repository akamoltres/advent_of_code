#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_DIM_STR_ARR 200

int read_csv_line(const char *filename, long *buffer, int bufsize);

int read_str_lines(const char *filename, char array_in[MAX_DIM_STR_ARR][MAX_DIM_STR_ARR]);

#ifdef __cplusplus
}
#endif
