#ifndef __USART_H
#define __USART_H

#define BAUDRATE B9600 							//Baud rate :9600
#define DEVICE "/dev/ttyAMA2"

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

void sendstr(char **pstr,int a,int b);
int sendData(int S,int P);
//int sendChar(char str);

//Open Port & Set Port
#endif
