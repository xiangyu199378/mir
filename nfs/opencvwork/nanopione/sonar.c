#include "sonar.h"

struct rGPIO *GPIOB;
double altitude()
{
	double result;
	struct timeval t1, t2;
	init();
	GPIOB->GPIO_OUT = 0x0;
	GPIOB->GPIO_OUT &= ~(0x01 <<31); 	//GPIOD21输出低电平
	GPIOB->GPIO_OUT = 0x01 << 31;		//GPIOD21输出高电平
	usleep(20);							//输出高电平20us
	GPIOB->GPIO_OUT &= ~(0x01 <<31); 	//GPIOD21输出低电平

	while(!(GPIOB->GPIO_PAD & (0x01 << 30)));	//等待GPIOD17输入高电平
	gettimeofday(&t1, NULL);

	while(GPIOB->GPIO_PAD & (0x01 << 30));		//等待GPIO17输入低电平
	gettimeofday(&t2, NULL);
//	printf("sec:%d,usec:%d\n",t2.tv_sec - t1.tv_sec, t2.tv_usec - t1.tv_usec);
	result = ((t2.tv_sec - t1.tv_sec) * 170.0 * 100.0 + (t2.tv_usec - t1.tv_usec) * 170.0 / 10000.0);  // 170m/s  /  1000000us * 100cm
	//printf("distance: %2.2fcm\n", result);
	deinit();
	return result;	
}

void init()
{   
	GPIOB = GetGPIOB();  
	if((int)GPIOB < 0)
			exit(EXIT_FAILURE);
	GPIOB->GPIO_DETMODE1 |= (0x11 << 10) | (0x11 << 2); //检测模式寄存器10 11位   2  3位置1   上升沿有效
	GPIOB->GPIO_OUTENB |= 0x01 << 31;   //输出使能寄存器第21位置1  GPIO21变为输出模式	
	GPIOB->GPIO_OUTENB &= ~(0x01 << 30);//输出使能寄存器第17位置0  GPIO17变为输入模式
}  

void deinit()
{		
	GPIOB->GPIO_OUTENB &= ~(0x01 <<31); //输出使能寄存器第21位置0 
	GPIOB->GPIO_DETMODE1 &= ~(0x11 << 10); //检测模式寄存器10 11位 置0    低电平有效
	FreeMmap((int *)GPIOB);	//释放映射到内存的区域
}
