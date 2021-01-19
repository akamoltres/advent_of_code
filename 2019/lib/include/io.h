#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

int read_csv_line(const char *filename, long *buffer, int bufsize);

#ifdef __cplusplus
}
#endif
