#ifndef USERSPACE_H
#define USERSPACE_H

struct task* make_userspace_process(char* elfStart, char* elfEnd, char* command_line);

#endif // USERSPACE_H