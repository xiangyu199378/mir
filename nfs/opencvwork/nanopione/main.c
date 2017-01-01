
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

  #include "usart/usart.h"
  #include "sonar/sonar.h"



int main(void)
{
  int fd ;
  init();
    char * leds = "/dev/leds";
    if((fd = open(leds,O_RDWR|O_NOCTTY|O_NDELAY))<0)
            printf("open%s failed\n",leds);
	else
          {
          ioctl(fd,1,1);
          printf("open%s success\n",leds);    
          }

return 0;	
}
