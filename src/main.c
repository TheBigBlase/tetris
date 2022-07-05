#include <stdio.h>

#include "lib.h"

int main(int argc, const char* argv[])
{
  struct library lib = create_library();

  (void)argc;
  (void)argv;

  printf("Hello from %s!", lib.name);
  return 0;
}
