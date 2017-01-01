#include "GPIO.h"
#include <stdio.h>
struct rGPIO* GetGPIOD()
{
	return (struct rGPIO *)GetMmap_(BA_GPIO + Offset_D);
}

struct rGPIO* GetGPIOB()
{
	return (struct rGPIO *)GetMmap_(BA_GPIO + Offset_B);
}

struct rGPIO* GetGPIOC()
{
	return (struct rGPIO *)GetMmap_(BA_GPIO + Offset_C);
}

int GPIO_OUTMODE(struct rGPIO* GPIO,...)				//可变参数，第一个参数为每组GPIO的起始地址，类型为rGPIO结构体指针
{														//后面的参数全为int，是GPIO编号，如GPIO2就输入2，表示本组GPIO2置为输出模式
	va_list ap;
	va_start(ap, GPIO);
	int i;
	if(NULL == GPIO)	//	当传入的GPIO指针为空就返回gpioIsNull
		return gpioIsNull;
	while(1)
	{
		i = va_arg(ap, int);
		if(i == DEF)
		{
			break;
		}
		GPIO->GPIO_OUTENB |= 0x1 << i;		//对应的GPIO_OUTENB位置1
	}
	va_end(ap);
	return 1;								//置位成功返回1
}	

int GPIO_OUTH(struct rGPIO* GPIO, ...)			//对应GPIO输出1，高电平
{
	va_list ap;
	va_start(ap, GPIO);
	int i;
	if(NULL == GPIO)	//	当传入的GPIO指针为空就返回gpioIsNull
		return gpioIsNull;
	while(1)
	{
		i = va_arg(ap, int);
		if(i == DEF)
		{
			break;
		}
		GPIO->GPIO_OUT |= 0x1 << i;		//对应的GPIO_OUT位置1
	}
	va_end(ap);
	return 1;	
}

int GPIO_OUTL(struct rGPIO* GPIO, ...)			//对应GPIO输出低电平
{
	va_list ap;
	va_start(ap, GPIO);
	int i;
	if(NULL == GPIO)	//	当传入的GPIO指针为空就返回gpioIsNull
		return gpioIsNull;;
	while(1)
	{
		i = va_arg(ap, int);
		if(i == DEF)
		{
			break;
		}
		GPIO->GPIO_OUT &= ~(0x1 << i);		//对应的GPIO_OUT位置0
	}
	va_end(ap);
	return 1;	
}

int GPIO_INMODE(struct rGPIO* GPIO, ...)
{
	va_list ap;
	va_start(ap, GPIO);
	int i;
	if(NULL == GPIO)	//	当传入的GPIO指针为空就返回gpioIsNull
		return gpioIsNull;
	while(1)
	{
		i = va_arg(ap, int);
		if(i == DEF)
		{
			break;
		}
		GPIO->GPIO_OUTENB &= ~(0x1 << i);		//对应的GPIO_OUTENB位置1
	}
	va_end(ap);
	return 1;	
}

int GetGPIOvalue(struct rGPIO* GPIO, int GPIOnum)	//获取对应GPIO的值
{
	if(NULL == GPIO)	//	当传入的GPIO指针为空就返回gpioIsNull
		return gpioIsNull;
	return GPIO->GPIO_PAD & (0x1 << GPIOnum);
}
