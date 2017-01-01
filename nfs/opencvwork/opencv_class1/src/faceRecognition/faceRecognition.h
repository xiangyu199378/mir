#ifndef __FACERECOGNITION_H
#define __FACERECOGNITION_H
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


using namespace std;
using namespace cv;

int faceRecognition(void);                   //人脸识别入口函数
void detectAndDisplay( Mat frame );


#include <iostream>
#include <stdio.h>

#endif
