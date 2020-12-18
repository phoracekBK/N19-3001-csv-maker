#include "include/csv_io.h"



char * csv_io_generate_csv_name(time_t t, char * csv_path, char * csv_name)
{
  	struct tm tm = *localtime(&t);
	return c_string_format("%s/%s-%d-%02d-%02d.csv", csv_path, csv_name, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
}


bool csv_io_write_to_file(char * csv_path, char * format, ...)
{
  FILE * file = fopen(csv_path, "a");

  if(file != NULL)
  {
	va_list args;
	va_start(args, format);

    vfprintf(file, format, args);

    fflush(file);
    fclose(file);
	va_end(args);

    return true;
  }

  return false;
}
