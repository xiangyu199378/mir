#ifndef GPIO_R_H_
#define GPIO_R_H_

#include "S5P4418.h"
#include <stdarg.h>

struct rGPIO
{
	int GPIO_OUT;	//output register GPIO输出寄存器
	int GPIO_OUTENB;//output enable register GPIO使能寄存器
	int GPIO_DETMODE0;//event detect mode register0 事件检测模式寄存器0
	int GPIO_DETMODE1;
	int GPIO_INTENDB;//interrupt enable register中断使能寄存器
	int GPIO_DET;//event detect register事件检测寄存器
	int GPIO_PAD;//PAD status register PAD状态寄存器
	int RSVD;//	Reserved	保留
	int GPIO_ALTFN0;//GPIO alternate function select register 0 GPIO 替代功能寄存器
	int GPIO_ALTFN1;
	int GPIO_DETMODEEX;//event detect mode extended register GPIO事件检测模式扩展寄存器
	int GPIO_DETENB;//detect enable register 检测使能寄存器
	int GPIO_SLEW;//slew register 转换寄存器
	int GPIO_SLEW_DISABLE_DEFAULT;//slew disable default register转换禁用默认寄存器
	int GPIO_DRV1;//DRV1 register DRV1寄存器
	int GPIO_DRV1_DISABLE_DEFAULT;//DRV1 disable default register DRV1禁用默认寄存器
	int GPIO_DRV0;//DRV0 register DRV0 寄存器
	int GPIO_DRV0_DISABLE_DEFAULT;//DRV0 disable defuault register DRV0禁用默认寄存器
	int GPIO_PULLSEL;//pull select register 上下拉选择寄存器
	int GPIO_PULLENB_DISABLE_DEFAULT;//PULLENB disable default register 上下拉禁用默认寄存器
};

#define Offset_A 0xA000 
#define Offset_B 0xB000 
#define Offset_C 0xC000 
#define Offset_D 0xD000 
#define Offset_E 0xE000 
#define BA_GPIO 0xC0010000

#define DEF (-1)
#define dGPIO_OUTMODE(GPIO, ...) GPIO_OUTMODE(GPIO, __VA_ARGS__, DEF)
#define dGPIO_OUTH(GPIO, ...) GPIO_OUTH(GPIO,__VA_ARGS__, DEF)
#define dGPIO_OUTL(GPIO, ...) GPIO_OUTL(GPIO, __VA_ARGS__, DEF)
#define dGPIO_INMODE(GPIO, ...) GPIO_INMODE(GPIO, __VA_ARGS__, DEF)

struct rGPIO* GetGPIOD();


struct rGPIO* GetGPIOB();

struct rGPIO* GetGPIOC();

int GPIO_OUTMODE(struct rGPIO* GPIO,...);	//可变参数，第一个参数为每组GPIO的起始地址，类型为rGPIO结构体指针

int GPIO_OUTH(struct rGPIO* GPIO, ...);	//对应GPIO输出1，高电平

int GPIO_OUTL(struct rGPIO* GPIO, ...);	//对应GPIO输出低电平


int GPIO_INMODE(struct rGPIO* GPIO, ...);


int GetGPIOvalue(struct rGPIO* GPIO, int GPIOnum);	//获取对应GPIO的值

#endif
