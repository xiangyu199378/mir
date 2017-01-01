
#include "../cardrive/cardrive.hpp"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#include "../lib/includes/libfahw.h"
mir_cardrive car;

#define DRIVER_MODULE1 "matrix_pwm"

mir_cardrive::mir_cardrive()
{     
	 
	
}


int mir_cardrive::init(void)
{
     int  board;
    
     Hz =1000;
     
     ret =-1;
	 pin1 = GPIO_PIN(29); 
     pin2 = GPIO_PIN(31); 
     
     pwm0=0;
     pwm1=1;
     
    if ((board = boardInit()) < 0) {
        printf("Fail to init board\n");
        return -1;
    }
  
    
     init_gpio();
     init_pwm();
     
 return 0;  
}
int mir_cardrive::init_gpio(void)
{
	 int ret1;
	 
	if ((ret1 = exportGPIOPin(pin1)) == -1) {   
        printf("exportGPIOPin(%d) failed\n", pin1);
    }
    if ((ret1 = setGPIODirection(pin1, GPIO_OUT)) == -1) {
        printf("setGPIODirection(%d) failed\n", pin1);
    } 
		if ((ret1 = exportGPIOPin(pin2)) == -1) {   
        printf("exportGPIOPin(%d) failed\n", pin2);
    }
    if ((ret1 = setGPIODirection(pin2, GPIO_OUT)) == -1) {
        printf("setGPIODirection(%d) failed\n", pin2);
    } 
   
    	level_gpio(GPIO_LOW);
 
    return ret1;
}

int mir_cardrive::release(void)
{
    unexportGPIOPin(pin1);
    unexportGPIOPin(pin2);

    PWMStop(pwm0);
    PWMStop(pwm1);
    
    system("rmmod "DRIVER_MODULE1);	

}
int mir_cardrive::init_pwm(void)
{    
	  int ret2;
	   system("modprobe "DRIVER_MODULE1); 
	    duty =0;
	if ((ret2=PWMPlay(pwm0, Hz, duty)) == -1) 
	    printf("Fail to output PWM\n");
	     
	    duty =0;
	if ((ret2=PWMPlay(pwm1, Hz, duty)) == -1) 
	    printf("Fail to output PWM\n");
	    
return ret;
}
void mir_cardrive::level_gpio(int val)
 {
	 
    if (setGPIOValue(pin1, val)> 0) {
		printf(" GPIO_PIN(%d) value is %d\n" , pin1, val);
	} else {
		printf("setGPIOValue(%d) failed\n", pin1);
	}
	 if ( setGPIOValue(pin2, val)> 0) {
		printf(" GPIO_PIN(%d) value is %d\n" , pin2, val);
	} else {
		printf("setGPIOValue(%d) failed\n", pin2);
	}
}


void mir_cardrive::left(void)
{
	 level_gpio(GPIO_LOW);
    duty =100;
	 if (PWMPlay(pwm0, Hz, duty) == -1) 
	    printf("Fail to output PWM\n");
	    
	 duty =500;
	if (PWMPlay(pwm1, Hz, duty) == -1) 
	    printf("Fail to output PWM\n");	
}


void mir_cardrive::right(void)
{
	   level_gpio(GPIO_LOW);
       duty =500;
	 if (PWMPlay(pwm0, Hz, duty) == -1) 
	    printf("Fail to output PWM\n");
	   
	    duty =100;
	if (PWMPlay(pwm1, Hz, duty) == -1) 
	    printf("Fail to output PWM\n");	
}

 

void mir_cardrive::up(void)
{

     level_gpio(GPIO_LOW);
	  duty =1000-600;
	 if (PWMPlay(pwm0, Hz, duty) == -1) printf("Fail to output PWM\n");
	  duty =1000-600;
   	if (PWMPlay(pwm1, Hz, duty) == -1)    printf("Fail to output PWM\n");	 
}


void mir_cardrive::down(void)
{
        
       level_gpio(GPIO_HIGH);
	   duty =600;
	 if (PWMPlay(pwm0, Hz, duty) == -1)   printf("Fail to output PWM\n");
	   duty =600;
   	if (PWMPlay(pwm1, Hz, duty) == -1)    printf("Fail to output PWM\n");	
}
void mir_cardrive::stop(void)
{
      level_gpio(GPIO_LOW);
      duty =0;
	 if (PWMPlay(pwm0, Hz, duty) == -1) 
	    printf("Fail to output PWM\n");
	  duty =0;
	if (PWMPlay(pwm1, Hz, duty) == -1) 
	    printf("Fail to output PWM\n");	 
}
	






