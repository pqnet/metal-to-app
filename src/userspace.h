#ifndef USERSPACE_H
#define USERSPACE_H

struct task* make_userspace_process(char* elfStart, char* elfEnd);

#endif // USERSPACE_H