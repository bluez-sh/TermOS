#ifndef LIBC_FILE_H
#define LIBC_FILE_H

#define MAX_FILE_SIZE 1024

int read_file(int fd, char *buf);
int write_file(int fd, char *buf);

#endif
