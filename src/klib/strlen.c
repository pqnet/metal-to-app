#include <string.h>

size_t strlen(const char *__s)
{
  const char *end = __s;
  while (*end)
  {
    end++;
  }
  return end - __s;
}