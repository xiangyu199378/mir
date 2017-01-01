#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <malloc.h>

#define BAUDRATE B9600 							//Baud rate :9600
#define DEVICE "/dev/ttyAMA2"

int nFd = 0;
struct termios stNew;
struct termios stOld;
void sendstr(char **pstr,int a,int b);
int sendData(int S,int P);
//int sendChar(char str);

//Open Port & Set Port
int SerialInit()
{
  nFd = open(DEVICE, O_RDWR|O_NOCTTY|O_NDELAY);
  if(-1 == nFd)
  {
    perror("Open Serial Port Error!\n");
    return -1;
  }
  if( (fcntl(nFd, F_SETFL, 0)) < 0 )
  {
    perror("Fcntl F_SETFL Error!\n");
    return -1;
  }
  if(tcgetattr(nFd, &stOld) != 0)
  {
    perror("tcgetattr error!\n");
    return -1;
  }

  stNew = stOld;
  cfmakeraw(&stNew);//将终端设置为原始模式，该模式下所有的输入数据以字节为单位被处理

  //set speed
  cfsetispeed(&stNew, BAUDRATE);//115200
  cfsetospeed(&stNew, BAUDRATE);

  //set databits  
  stNew.c_cflag |= (CLOCAL|CREAD);
  stNew.c_cflag &= ~CSIZE;
  stNew.c_cflag |= CS8;

  //set parity  
  stNew.c_cflag &= ~PARENB;  
  stNew.c_iflag &= ~INPCK;

  //set stopbits  
  stNew.c_cflag &= ~CSTOPB;
  stNew.c_cc[VTIME]=0;	//指定所要读取字符的最小数量
  stNew.c_cc[VMIN]=1;	//指定读取第一个字符的等待时间，时间的单位为n*100ms
        //如果设置VTIME=0，则无字符输入时read（）操作无限期的阻塞
  tcflush(nFd,TCIFLUSH);	//清空终端未完成的输入/输出请求及数据。
  if( tcsetattr(nFd,TCSANOW,&stNew) != 0 )
  {
    perror("tcsetattr Error!\n");
    return -1;
  }

  return nFd;
}


int sendData(int S,int P)
{
	int nRet = 0,i;
	int SIZE;
	char *Data=NULL;
	if( SerialInit() == -1 )
	{
		//perror("SerialInit Error!\n");
		return -1;
	}
	sendstr(&Data,S,P);
	SIZE = strlen(Data);
		nRet = write(nFd,Data,SIZE);
		nRet = write(nFd,"\r\n",2);
  close(nFd);
  return 0;
}

void sendstr(char **pstr,int a,int b)
{
	int i=0;
	int len1=0,len2=0;
	char *str1 = (char *)malloc(5),*str2 = (char *)malloc(7);

	sprintf(str1,"%d",a);
	sprintf(str2,"%d",b);

	len1 = strlen(str1);
	len2 = strlen(str2);
	
	for(i = len1;i >= 0;i--)  {
		str1[i+1] = str1[i];
		}
		
	str1[0] = 'S';

	//printf("str1 = %s\n",str1);


	for(i = len2-1;i >= 0;i--) {
		str2[i+1] = str2[i];
	}
	str2[0] = 'P';
	str2[len2+1] = 'E';
	str2[len2+2] = '\0';
	
	//str2[len2+2] = 0x0D;
	//str2[len2+3] = 0x0A;
	//printf("str2 = %s\n",str2);

	*pstr = strcat(str1,str2);	
	//printf("*pstr = %s\n",*pstr);
}

