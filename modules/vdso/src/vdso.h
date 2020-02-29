#ifndef VDSO_H
#define VDSO_H
#include <stdint.h>
#include <stddef.h>

void abort();
void _exit(int status);
int64_t write(int fd, const void *buf, size_t count);

#endif // VDSO_H