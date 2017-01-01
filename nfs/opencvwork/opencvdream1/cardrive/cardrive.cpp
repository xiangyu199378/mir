
#include"../cardrive/cardrive.hpp"


mir_cardrive car;



mir_cardrive::mir_cardrive()
{
	
	
}

void mir_cardrive::init(void)
{
	
	 
	GPIOC = GetGPIOC();  
//	GPIOC->GPIO_DETMODE0 |= (0x11 << 10) | (0x11 << 2); //检测模式寄存器10 11位   2  3位置1   上升沿有效
	GPIOC->GPIO_OUTENB   |= 0x0000F000;                 //输出使能寄存器第10 11 12 13 位置1  
//	GPIOC->GPIO_OUTENB   |= 0x0F << 10;                 //输出使能寄存器第10 11 12 13 位置1  

}



void mir_cardrive::left(void)
{
//1000	
	GPIOC->GPIO_OUT = 0x1<< 10;		//GPIOC 10 输出高电平
	GPIOC->GPIO_OUT &= ~(0x1<<10); 	//GPIOC 11 12 13输出低电平
//	GPIOC->GPIO_OUT &= ~(0x1<<10); 	//GPIOC 11 12 13输出低电平
}


void mir_cardrive::right(void)
{
//0001	
    GPIOC->GPIO_OUT  = 0x8 << 10;		//GPIOC 13输出高电平
	GPIOC->GPIO_OUT &= ~(0x8<<10); 	    //GPIOC 10 11 12输出低电平
	
}


void mir_cardrive::up(void)
{
//1010	
	
	GPIOC->GPIO_OUT = 0x6<< 10;		//GPIOC 10 12输出高电平
    GPIOC->GPIO_OUT &= ~(0x6 <<10); 	//GPIOC 11 13输出低电平	
}


void mir_cardrive::down(void)
{
//0101	
	GPIOC->GPIO_OUT  = 0xA<< 10;		//GPIOC 10 12输出高电平
	GPIOC->GPIO_OUT &= ~(0xA<<10); 	//GPIOC 11 13输出低电平
}

void mir_cardrive::stop(void)
{
//0101	
	GPIOC->GPIO_OUT  =0;		//GPIOC 10 12输出高电平
	
}





