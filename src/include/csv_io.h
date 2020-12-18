#ifndef _CSV_IO_H_
#define _CSV_IO_H_

#include <c_string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

char * csv_io_generate_csv_name(time_t, char *, char *);
bool csv_io_write_to_file(char *, char *, ...);

#endif
