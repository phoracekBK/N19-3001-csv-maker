#include <stdlib.h>
#include <stdio.h>
#include <s7lib.h>

int main(int argv, char ** argc)
{
  s7lib * s7lib_ref = s7lib_new("192.168.2.1", 0, 1, 2);

  uint8_t * buffer = s7lib_read(s7lib_ref, 0, 4);
  float real_value = 0;
  ((uint8_t*) &real_value)[0] = buffer[3];
  ((uint8_t*) &real_value)[1] = buffer[2];
  ((uint8_t*) &real_value)[2] = buffer[1];
  ((uint8_t*) &real_value)[3] = buffer[0];

  float value = s7lib_read_real(s7lib_ref, 0);

  printf("%f : %f\n", real_value, value);

  s7lib_write_real(s7lib_ref, 4, 114.7);

  s7lib_finalize(s7lib_ref);
  return 0;
}
