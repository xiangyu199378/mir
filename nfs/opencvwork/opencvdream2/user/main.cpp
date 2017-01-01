#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp> 
//#include "../sonar/sonar.h"
//#include "../usart/usart.h"
#include "../cardrive/cardrive.hpp"
#include "../lib/includes/libfahw.h"
#include "../tracking/featuretracking.hpp"
#include "../line_detection/line_detection.h"
#include "../videotransmission/SocketVideoserver.hpp"


using namespace cv;
/*----------------------------------------------*/
	Mat frame;
	Mat result;
	static int pwm;
/*---------------------------------------------*/
void shownhelp(void);

int main(void)
{
  shownhelp();	
  car.init();
  track.test_org();
  
 // line_detection();
 // socketVideo.test();
 
 cout << "\nThe  program  is over \n"<< endl;	
return 0;	
}

void shownhelp(void)
{
	 cout << "\nThe main program  is my congraduratin project .\n"<< endl;
}


