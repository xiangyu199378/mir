#include "S5P4418.h"
int* GetMmap(int baseAddr, int dev_fd)
{
	return (int *)mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, baseAddr) ;
}

void FreeMmap(int *mmap)
{
		munmap(mmap, getpagesize());
}

int OpenMem()
{
	int dev_fd = open("/dev/mem", O_RDWR |O_NDELAY);
	if(dev_fd < 0)
	{
			return openMenFailed;
	}
	else
		return dev_fd;
}


int* GetMmap_(int BaseAddr)
{
	int* mmap = NULL;
	int dev_fd = OpenMem();
	mmap = GetMmap(BaseAddr, dev_fd);
	close(dev_fd);
	return mmap;
}
