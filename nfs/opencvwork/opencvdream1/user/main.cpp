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




using namespace cv;

/*---------------------------------------------*/

void shownhelp(void);


int main(void)
{
  shownhelp();	


 	 line_detection();
 
  
  
  	
  return 0;	
}



void shownhelp(void)
{
	 cout << "\nThe main program  is my congraduratin project .\n"<< endl;
}


