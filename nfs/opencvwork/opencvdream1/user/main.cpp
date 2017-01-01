#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp> 
#include <cmath>

#include "../sonar/sonar.h"
#include "../usart/usart.h"
#include "../line_detection/line_detection.h"

#include "../cardrive/cardrive.hpp"

#include "../lib/common.h"
#include "../lib/includes/libfahw-filectl.h"

using namespace cv;
/*----------------------------------------------*/
	Mat frame;
	Mat result;
/*---------------------------------------------*/
void shownhelp(void);

int main(void)
{
  shownhelp();	
  sonar_init();
  car.init();
//  VideoCapture capture(0);//打开摄像头

while(true)
{
//	    car.down();	  
//		sleep(5);	
//		car.up();
//		sleep(5);	
car.stop();
}
       
/*
	if(capture.isOpened())	// 摄像头读取文件开关
	{
		while(true)
		{
			capture >> frame;
			
			if(!frame.empty())
			{
				//frame = canny(frame);
				detection(frame, result);
			}
			else
			{ 
				printf(" --(!) No captured frame -- Break!");
				
				 break;
			}
		  altitude();
		
		}
		
		
 // if(line_detection());

} */
  
 cout << "\nThe  program  is over \n"<< endl;
  	
  return 0;	
}



void shownhelp(void)
{
	 cout << "\nThe main program  is my congraduratin project .\n"<< endl;
}


