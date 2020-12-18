#include <stdio.h>
#include <stdlib.h>
#include "unit_test.h"

int main(int argv, char ** argc)
{
  printf("----- Unit test start -----\n");
  unit_test_run();
  printf("------ Unit test end ------\n");

  return 0;
}
