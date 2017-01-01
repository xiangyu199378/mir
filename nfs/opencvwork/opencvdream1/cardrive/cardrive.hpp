

#ifndef __LINE_DETECTION_HPP
#define __LINE_DETECTION_HPP


#include "../nanopi/GPIO.h"

class mir_cardrive
{
	
public:
mir_cardrive();

struct rGPIO *GPIOC;

void init(void);
void left(void);
void right(void);
void up(void);
void down(void);	
void stop(void);
};

extern mir_cardrive car;
/*
#define PWM_IOCTL_SET_FREQ      (0x1)
#define PWM_IOCTL_STOP          (0x0)
#define PWM_IOCTL_CONFIG        (0x4)
#define PWM_IOCTL_RELEASE       (0x8)

int initPwmGPIO(int board);
int PWMPlay(int pin, int freq, int duty);
int PWMStop(int pin);
*/

#endif
