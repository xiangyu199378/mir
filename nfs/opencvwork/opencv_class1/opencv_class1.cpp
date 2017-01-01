#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/video/background_segm.hpp> 
#include <cmath>

#include "src/faceRecognition/faceRecognition.h"
#include "src/line_detection/line_detection.h"
#include "src/opticalFlow/opticalFlow.h" 
#include "src/videoprocessing/videoprocessing.h"
#include "src/foreground/foreground.h"
//#include "src/tracking/tracking.h"
#include "src/featureMatch/featureMatch.h"
#include "src/usart/usart.h"
#include "src/sonar/sonar.h"


#define MOUSE_KEY 0
#define COLOR_KEY 0
#define TRACK_KEY 1
#define LINES_KEY 0
#define FEATURE_KEY 0

using namespace cv; 
using namespace std;

Mat frame;						//保存帧图像
Point origin;					//用于保存鼠标选择第一次单击时点的位置
Rect selection;					//用于保存鼠标选择的矩形框
bool selectObject = false;		//代表是否在选要跟踪的初始目标，true表示正在用鼠标选择
bool sign=false;
extern int frame_x;
extern int frame_y;
double old_distance = 0;		//上一时刻位移
double velocity = 0;			//移动速度
float Xn = 0;
float Yn = 0;
float Length = 10;

void salt(cv::Mat image, int n);
void film(void);
void showImage(void);
float mat_data(int x_rel, int y_rel);
void coordinate(int angle);
int colorDertificate(void);

void onMouse( int event, int x, int y, int, void*);
cv::Mat canny(cv::Mat image);


int main()  
{  	
	int flow_data=0;
	int ise = 0;
	int count=0;
	double buf_angle[10]={0};
	Mat frame2;
	Mat result;

	          
	#if MOUSE_KEY
		bool stop = false;
		bool write_image=false;
		Mat ROI;
		VideoCapture capture(0);

		if(!capture.isOpened())
		{
		    return -1;
		}

		namedWindow("frame",0);
		setMouseCallback( "frame", onMouse, 0 );//消息响应机制

		while(!stop || !write_image)
		{
		    capture>>frame;
		
			if( selectObject && selection.width > 0 && selection.height > 0 )
			{
			    ROI = frame(selection);//这句话是将frame帧图片中的选中矩形区域的地址指向ROI，
			                           //对于内存而言，frame和ROI是公用内存的，所以下面这句实际
			                           //是将frame帧图像中的选中矩形区域块图像进行操作，而不是新创建
			                           //一个内存来进行操作
			    //当然所截图的矩形区域ROI，可以使用imwrite函数来保存
				cv::imwrite("src/featureMatch/dawn.jpg",ROI);
			    bitwise_not(ROI, ROI);//bitwise_not为将每一个bit位取反
				write_image=true;

			} 
			imshow("frame",frame);
			if( waitKey(30) == 27 )//ESC键退出
		    stop = true;	
		}
		sign=true;
		if(sign) {
			//VideoCapture capture(0);//打开摄像头0
			if(!capture.isOpened())	{
				return -1;
			}	else  {
							while(true)	{
								capture >> frame2;
								if(!frame2.empty())	{
									featurePicture(frame2);    //图像识别
								}
								else	
								{ 
									printf(" --(!) No captured frame -- Break!");
									break;
								}
								int c = waitKey(5);
								if( (char)c == 27 )	{
									break; 
								} 
							}		
				}
		}
	#else
	VideoCapture capture(0);//打开摄像头0

	if(capture.isOpened())	{
		while(true)	{
			capture >> frame2;

			if(!frame2.empty())	{
				
				#if TRACK_KEY
				tracking(frame2, result);
				flow_data=mat_data(frame_x,frame_y);
				count++;
				if(count>9) count=0;

				buf_angle[count]=mat_data(frame_x,frame_y);
				flow_data=StatisticAngle(buf_angle);
				coordinate(flow_data);
				printf("%d\t(%f,%f)\t%f\n",flow_data,Xn,Yn,altitude());
				//printf("%d\t(%f,%f)\n",flow_data,Xn,Yn);
				//sendData(flow_data,ise);
				#endif
				#if FEATURE_KEY
				featurePicture(frame2);
				#endif
				#if LINES_KEY
				detection(frame2,result);
				#endif
				#if COLOR_KEY
				cv::imwrite("src/featureMatch/dawn.jpg",frame2);      //如果使用颜色识别，请去掉注释
				colorDertificate();
				#endif
			}
			else	{ 
				printf(" --(!) No captured frame -- Break!");
				break;
			}
			int c = waitKey(5);
			if( (char)c == 27 )		{
				break; 
			} 
		}
	}
	#endif
	return 0;
}


float mat_data(int x_rel, int y_rel) {
	int i=0;
	double distance=0,position=0;

	if(abs(x_rel)>10 && abs(y_rel)>10) {
		distance = InvSqrt(x_rel*x_rel+y_rel*y_rel);
		if(distance<20 || distance>715) {
			distance=0;
		}
		velocity = distance - old_distance;
		position = (double)(y_rel/distance);
		position = (asin(position)*180)/CV_PI;
		if(x_rel<0 && y_rel>0) {
			position =180-position;
		} else if(x_rel<0 && y_rel<0) {
			position = -position-180;
		} 
	} else if(x_rel==0 && y_rel>0) {
		position = 90;
	} else if(x_rel==0 && y_rel<0) {
		position = -90;
	} else if(x_rel>0 && y_rel==0) {
		position = 0;
	} else if(x_rel<0 && y_rel==0) {
		position = 180;
	}
		//printf("X is :%d,Y is :%d\n",x,y);
	position = -position;     
	//if(FlowReflash)	{
		
	//}
	old_distance = distance;
	if(velocity!=0)	{
		//printf("%f\n",velocity*0.2);
	}
	return position;	
}

void coordinate(int angle) {
	if(angle!=0) {
		Xn = Xn + Length * cos(angle);
		Yn = Yn + Length * sin(angle);
	}
}


void onMouse( int event, int x, int y, int, void* )
{
    if( selectObject )//只有当鼠标左键按下去时才有效，然后通过if里面代码就可以确定所选择的矩形区域selection了
    {
        selection.x = MIN(x, origin.x);//矩形左上角顶点坐标
        selection.y = MIN(y, origin.y);
        selection.width = std::abs(x - origin.x);//矩形宽
        selection.height = std::abs(y - origin.y);//矩形高

        selection &= Rect(0, 0, frame.cols, frame.rows);//用于确保所选的矩形区域在图片范围内
    }

    switch( event )
    {
    case CV_EVENT_LBUTTONDOWN:
        origin = Point(x,y);
        selection = Rect(x,y,0,0);//鼠标刚按下去时初始化了一个矩形区域
        selectObject = true;
        break;
    case CV_EVENT_LBUTTONUP:
        selectObject = false;
        if( selection.width > 0 && selection.height > 0 )
        break;
    }
}




int colorDertificate(void)
{
  CvScalar pixel;
  int i = 0, j = 0;
  int numw = 0, numb = 0;

  IplImage* img = cvLoadImage("/home/dawn/workspace/opencv_class1/opencv_class1/src/featureMatch/dawn.jpg", 
        CV_LOAD_IMAGE_ANYCOLOR|CV_LOAD_IMAGE_ANYDEPTH);
  if(NULL == img)
  {
    printf("Load Image Fail!\n");
    return 2;
  }
//访问图像每一个像素点
  for(i=0; i<img->height; i++)
  {
    for(j=0; j<img->width; j++)
    {
      pixel = cvGet2D(img, i, j);
      if(pixel.val[0] > 128)
      {
        pixel.val[0] = 255;//8位单通道图像在pixel.val[0]中存储像素
        ++numw;//统计白点个数
      }
      else
      {
        pixel.val[0] = 0;
        ++numb;//统计黑点个数
      }
      cvSet2D(img, i, j, pixel);
    }
  }

  printf("Image Height:%d, Width:%d\n", img->height, img->width);
  printf("White num:%d, Black num:%d\n", numw, numb);

  cvSaveImage("Handle.jpg", img, 0);
  cvReleaseImage(&img);

  return 0;
}





	/*
	//cv::Mat image1 = cv::imread("1.jpg");
	//cv::Mat image2 = cv::imread("2.jpg");
	//Mat result;
	//tracking(image1, result);
	//tracking(image2, result);
	//colorReduce(image,64);
	//cv::namedWindow("Image");
	//cv::imshow("Image",image);
	//optialFlow();			//光流法测运动矢量
	//line_detection();		//直线检测
	//foreground();			//前景提取
	//videoprocessing();		//视频自定义帧处理
	//tracking();			//跟踪
	//featureMatch();			//图像识别匹配*/


