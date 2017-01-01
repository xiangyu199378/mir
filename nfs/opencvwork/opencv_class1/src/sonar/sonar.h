#ifndef __SONAR_H
#define __SONAR_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <malloc.h>
#include "GPIO.h"

double  altitude(void);
void init();
void deinit();

#endif
