#ifndef __OPTICALFLOW_H
#define __OPTICALFLOW_H

#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cstdio>
#include <cmath>


using namespace std;
using namespace cv;  



//-----------------------------------【全局函数声明】-----------------------------------------
//		描述：声明全局函数
//-------------------------------------------------------------------------------------------------
void optialFlow(void);              //光流法追踪的入口函数
void tracking(Mat &frame, Mat &output);
bool addNewPoints();
bool acceptTrackedPoint(int i);
float InvSqrt(float x);             //开平方
int StatisticAngle(double *angle);  //求分布最密集的角度
float standardDeviation(double *data);
void add_data(int *mat_x, int *mat_y, int *frame_x, int *frame_y);


#endif
