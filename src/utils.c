#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <unistd.h>
#include <execinfo.h>
#endif

char *copy_name (char *name)
{
  size_t len = strlen(name) + 1;
  char *out = malloc(sizeof(char) * len);
  strncpy(out, name, len);
  return out;
}

void print_backtrace ()
{
#ifndef WIN32
  void *array[10];
  size_t size;

  size = backtrace(array, 10);

  backtrace_symbols_fd(array, size, STDERR_FILENO);
#endif /* WIN32 */
}
