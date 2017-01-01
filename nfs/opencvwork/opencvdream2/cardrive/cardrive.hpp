

#ifndef __CARDRIVE_HPP
#define __CARDRIVE_HPP




class mir_cardrive
{
private:	  
     int pwm1,pwm0,Hz, duty;  
	 int ret ;
     int pin1 ,pin2 ; 
     
     int init_gpio(void);
     int init_pwm(void);
     void level_gpio(int val);
    
public:

   mir_cardrive();
int init(void);
int release(void);
void left(void);
void right(void);
void up(void);
void down(void);	
void stop(void);

};

extern mir_cardrive car;


#endif
