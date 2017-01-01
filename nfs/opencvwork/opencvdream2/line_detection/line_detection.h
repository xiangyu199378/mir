#ifndef __LINE_DETECTION_H
#define __LINE_DETECTION_H

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <math.h>
#include <stdio.h>

#include <iostream>

using namespace std;
using namespace cv;

void help(void);
int line_detection(void);
void detection(Mat &frame, Mat &output);
int distanceFilter(int *distance);

#endif
