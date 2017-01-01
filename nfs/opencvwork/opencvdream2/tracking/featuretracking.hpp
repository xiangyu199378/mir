#ifndef  __FEATURETACKING_HPP
#define  __FEATURETACKING_HPP

#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "opencv2/legacy/legacy.hpp"

#include <iostream>
#include <cmath>
#include <math.h>
#include <stdio.h>

using namespace std;
using namespace cv;

class mir_tracking
{
public:
 mir_tracking();
~mir_tracking();

int getORB(Mat &img_1 ,Mat &img_2);	
int featurePicture(Mat img_1,Mat img_2);
int test_org(void);
};
extern mir_tracking track;
#endif


