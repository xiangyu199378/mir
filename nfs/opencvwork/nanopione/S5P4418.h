#ifndef S5P4418_H_
#define S5P4418_H_

#include "common.h"
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#define U32 unsigned int
#define U8 unsigned char 
int* GetMmap(int baseAddr, int dev_fd);

void FreeMmap(int *mmap);

int OpenMem();

int* GetMmap_(int BaseAddr);
#endif
