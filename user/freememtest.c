#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int free_bytes = freemem();
  printf("Free memory: %d bytes (%d KB)\n", free_bytes, free_bytes/1024);
  exit(0);
}
