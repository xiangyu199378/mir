#ifndef __SONAR_H
#define __SONAR_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <malloc.h>
#include "../nanopi/GPIO.h"


 double altitude(void);
   void sonar_init(void);
   void sonar_deinit(void);





#endif
