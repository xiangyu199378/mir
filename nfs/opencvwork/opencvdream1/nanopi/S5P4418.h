#ifndef S5P4418_H_
#define S5P4418_H_


#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#define openMenFailed -2
#define getMapFailed -3
#define gpioIsNull -4

#define U32 unsigned int
#define U8 unsigned char 
int* GetMmap(int baseAddr, int dev_fd);

void FreeMmap(int *mmap);

int OpenMem();

int* GetMmap_(int BaseAddr);
#endif
