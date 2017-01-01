
/************************************************************************

* 文件:	line_detection.h
* 功能:  直线识别
* 作者:  Dawn
* 最后更改日期: 2016/5/25
* 注：x=L*h/B; x为实际长度,L为测得像素计算得到长度，h为高度，B为放大倍数，此摄像头B=125
************************************************************************/
#include "line_detection.h"
#include "../usart/usart.h"


int distance_buf[20];

int line_detection(void)
{
	Mat frame;
	Mat result;
	VideoCapture capture(0);//打开摄像头
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
		
		}
	}
	
	 
	return 0;
}



void detection(Mat &frame, Mat &output) {
	
	int x=0,y=0,num=0;
	float slope;
	int res=0;
	Mat dst, cdst;
	Canny(frame, dst, 50, 200, 3);
	cvtColor(dst, cdst, CV_GRAY2BGR);

	#if 0
	vector<Vec2f> lines;
	HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

	for( size_t i = 0; i < lines.size(); i++ )
	{ 
		float rho = lines[i][0], theta = lines[i][1];
		Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));

		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	}
	#else
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI/180, 50, 100, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		//printf("(X1:%d,Y1:%d)\n",l[0],l[1]);
		//printf("(X2:%d,Y2:%d)\n",l[2],l[3]);
		x = l[2]-l[0];
		y = l[3]-l[1];
		if(x!=0 && y!=0) {
			slope = (float) y/x;
			res=(240-l[1])/slope+l[0]-320;		//通过建立任意两点直线方程，取该直线与横向中线的交点坐标
			slope = atan(slope)*180/CV_PI;

		}	else if(x==0) 
			{	
				res = 90;
				slope = 0;
			}
		//printf("%f\n",slope);
	}
	#endif
	distance_buf[num]=res;
	num++;
	if(num>19) num=0;
	printf("%d\n",distanceFilter(distance_buf));
	imshow("windows", frame);
	imshow("Hello dawn", cdst);

	waitKey(3);
}

int distanceFilter(int *distance) {
	int i = 0,n = 0,mean=0;
	n = sizeof(distance);
	for(i=0;i<n;i++) {
		mean+=distance[i];
	}
	return mean/n;
}


/*
int line_detection(void)
{
	int x,y;
	float slope;
	 // const char* filename = argc >= 2 ? argv[1] : "line.bmp";

	 //Mat src = imread(filename, 0);
	 cv::Mat image = cv::imread("line3.bmp");
	 if(image.empty())
	 {
		 help();
		 cout << "can not open the file "  << endl;
		 return -1;
	 }

	 Mat dst, cdst;
	 Canny(image, dst, 50, 200, 3);
	 cvtColor(dst, cdst, CV_GRAY2BGR);

	 #if 0
	  vector<Vec2f> lines;
	  HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );

	  for( size_t i = 0; i < lines.size(); i++ )
	  {
		 float rho = lines[i][0], theta = lines[i][1];
		 Point pt1, pt2;
		 double a = cos(theta), b = sin(theta);
		 double x0 = a*rho, y0 = b*rho;
		 pt1.x = cvRound(x0 + 1000*(-b));
		 pt1.y = cvRound(y0 + 1000*(a));
		 pt2.x = cvRound(x0 - 1000*(-b));
		 pt2.y = cvRound(y0 - 1000*(a));
		 line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
	  }
	 #else
	  vector<Vec4i> lines;
	  HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
	  for( size_t i = 0; i < lines.size(); i++ )
	  {
		Vec4i l = lines[i];
		line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		printf("(X1:%d,Y1:%d)\n",l[0],l[1]);
		printf("(X2:%d,Y2:%d)\n",l[2],l[3]);
		x = l[2]-l[0];
		y = l[3]-l[1];
		if(x!=0) {
			slope = (float)(y/x);
			slope = atan(slope)*180/CV_PI;
			slope = -slope;
		}
		else slope = NULL;
		printf("%f\n",slope);
	  }
	 #endif
	 imshow("source", image);
	 imshow("detected lines", cdst);

	 waitKey(3);

	 return 0;
}
*/

void help(void)
{
	 cout << "\nThis program demonstrates line finding with the Hough transform.\n"
			 "Usage:\n"
			 "./houghlines <image_name>, Default is pic1.jpg\n" << endl;
}




