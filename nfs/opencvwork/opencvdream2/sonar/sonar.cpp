#include "sonar.h"

struct rGPIO *GPIOB;

static	double result;
	int _limit_cnt;

double altitude()
{
	struct timeval t1, t2;
	GPIOB->GPIO_OUT = 0x0;
	GPIOB->GPIO_OUT &= ~(0x01 <<31); 	//GPIOD31输出低电平
	GPIOB->GPIO_OUT = 0x01 << 31;		//GPIOD31输出高电平
	usleep(20);							//输出高电平20us
	GPIOB->GPIO_OUT &= ~(0x01 <<31); 	//GPIOD31输出低电平
    _limit_cnt=100;
	while(!(GPIOB->GPIO_PAD & (0x01 << 30))&&_limit_cnt--)usleep(2);		//等待GPIOD30输入高电平
	if(_limit_cnt>0)
	{
	gettimeofday(&t1, NULL);
    _limit_cnt=1000;
	while((GPIOB->GPIO_PAD & (0x01 << 30))&&_limit_cnt--)usleep(26);		//等待GPIO30输入低电平
	gettimeofday(&t2, NULL);
//	printf("sec:%d,usec:%d\n",t2.tv_sec - t1.tv_sec, t2.tv_usec - t1.tv_usec);
	result = ((t2.tv_sec - t1.tv_sec) * 170.0 * 100.0 + (t2.tv_usec - t1.tv_usec) * 170.0 / 10000.0);  // 170m/s  /  1000000us * 100cm
    printf("distance: %2.2fcm\n", result);
    }
//	sonar_deinit();
	return result;	
}

void sonar_init(void)
{   
	GPIOB = GetGPIOB();  
//	if((int)GPIOB < 0) exit(EXIT_FAILURE);
	GPIOB->GPIO_DETMODE1 |= (0x11 << 10) | (0x11 << 2); //检测模式寄存器10 11位   2  3位置1   上升沿有效
	
	GPIOB->GPIO_OUTENB   |= 0x01 << 31;                 //输出使能寄存器第31位置1  GPIO21变为输出模式	
	GPIOB->GPIO_OUTENB   &= ~(0x01 << 30);              //输出使能寄存器第30位置0  GPIO17变为输入模式
}  

void sonar_deinit(void)
{		
	GPIOB->GPIO_OUTENB &= ~(0x01 <<31); //输出使能寄存器第31位置0 
	GPIOB->GPIO_DETMODE1 &= ~(0x11 << 10); //检测模式寄存器10 11位 置0    低电平有效
	FreeMmap((int *)GPIOB);	//释放映射到内存的区域
}
