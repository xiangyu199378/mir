
/************************************************************************

* 文件:	opticalFlow.h
* 功能:  光流法运动轨迹解算
* 作者:  Dawn
* 最后更改日期: 2016/5/25
* 注：x=L*h/B; x为实际长度,L为测得像素计算得到长度，h为高度，B为放大倍数，此摄像头B=628
* 当前高度125cm；
************************************************************************/

#include "opticalFlow.h"

bool FlowReflash = false;

using namespace std;
using namespace cv;  
int frame_x=0;
int frame_y=0;


//-----------------------------------【全局变量声明】-----------------------------------------
//		描述：声明全局变量
//-------------------------------------------------------------------------------------------------
Mat gray;	// 当前图片
Mat gray_prev;	// 预测图片
vector<Point2f> points[2];	// point0为特征点的原来位置，point1为特征点的新位置
vector<Point2f> initial;	// 初始化跟踪点的位置
vector<Point2f> features;	// 检测的特征
vector<uchar> status;	    // 跟踪特征的状态，特征的流发现为1，否则为0
vector<float> err;


int maxCount = 100;         // 检测的最大特征数
double qLevel = 0.01;    	// 特征检测的等级
double minDist = 10.0;	    // 两特征点之间的最小距离


//-----------------------------------【optialFlow( )函数】--------------------------------------------
//		描述：控制台应用程序的入口函数，程序从这里开始
//-------------------------------------------------------------------------------------------------
/*void optialFlow(void) {

	Mat frame;
	Mat result;
	VideoCapture capture(0);//打开摄像头0
	//VideoCapture capture("2.mp4");

	if(capture.isOpened())	// 摄像头读取文件开关
	{
		while(true)
		{
			capture >> frame;

			if(!frame.empty())
			{
				//frame = canny(frame);
				tracking(frame, result);
			}
			else
			{ 
				printf(" --(!) No captured frame -- Break!");
				break;
			}

			int c = waitKey(5);
			if( (char)c == 27 )
			{
				break; 
			} 
			
		}
	}
}*/

//-------------------------------------------------------------------------------------------------
// 函数:   tracking
// 功能:   跟踪
// 参数:   frame	输入的视频帧
//		  output 有跟踪结果的视频帧
// 返回值: void
//-------------------------------------------------------------------------------------------------
void tracking(Mat &frame, Mat &output)
{
	double position=0;           //运动位置状态
	int x=0,y=0;
   // double angle[400];
	int mat_x[110]={0},mat_y[110]={0};
	double distance=0;
	float angle_rel=0;

	//此句代码的OpenCV3版为：
	//cvtColor(frame, gray, COLOR_BGR2GRAY);
	//此句代码的OpenCV2版为：
	cvtColor(frame, gray, CV_BGR2GRAY);

	frame.copyTo(output);

	// 添加特征点
	if (addNewPoints())
	{
		goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
		points[0].insert(points[0].end(), features.begin(), features.end());
		initial.insert(initial.end(), features.begin(), features.end());
		FlowReflash = true;
	}

	if (gray_prev.empty())
	{
		gray.copyTo(gray_prev);
	}
	// l-k光流法运动估计
	calcOpticalFlowPyrLK(gray_prev, gray, points[0], points[1], status, err);
	// 去掉一些不好的特征点
	int k = 0;
	for (size_t i=0; i<points[1].size(); i++)
	{
		if (acceptTrackedPoint(i))
		{
			initial[k] = initial[i];
			points[1][k++] = points[1][i];

		}
	}
	points[1].resize(k);
	initial.resize(k);
	// 显示特征点和运动轨迹
	for (size_t i=0; i<points[1].size(); i++)
	{
		line(output, initial[i], points[1][i], Scalar(0, 0, 255));
		circle(output, initial[i], 3, Scalar(0, 255, 0), -1);    //initial[i]为当前特征点
	
	
		x = (int)points[0][i].x-(int)points[1][i].x;
		y = (int)points[0][i].y-(int)points[1][i].y;
		mat_x[i]=x;
		mat_y[i]=y;
	}
	
	swap(points[1], points[0]);
	swap(gray_prev, gray);
	//imshow("hello dawn", output);
	add_data(mat_x, mat_y, &frame_x, &frame_y);
	//position = -position;
	//distance = distance*0.2;
	//printf("%f\n",position);
	//printf("%d\n",StatisticAngle(angle));
	//printf("%f\n",distance);
	//printf("%d,%d\n",x,y);
	//printf("Angle: %d\tDistance: %f\n",StatisticAngle(angle),distance);
	// 把当前跟踪结果作为下一此参考

}



void add_data(int *mat_x, int *mat_y, int *frame_x, int *frame_y) {
	int i=0,x_rel=0,y_rel=0;
	for(i=0;i<sizeof(mat_x);i++) {
		x_rel+=mat_x[i];
		y_rel+=mat_y[i];
	}
	*frame_x=x_rel;
	*frame_y=y_rel;	
}

//-------------------------------------------------------------------------------------------------
// 函数: addNewPoints
// 功能: 检测新点是否应该被添加
// 参数:
// 返回值: 是否被添加标志
//-------------------------------------------------------------------------------------------------
bool addNewPoints()
{
	return points[0].size() <= 10;
}

//-------------------------------------------------------------------------------------------------
// 函数: acceptTrackedPoint
// 功能: 决定哪些跟踪点被接受
// 参数:
// 返回值:
//-------------------------------------------------------------------------------------------------
bool acceptTrackedPoint(int i)
{
	return status[i] && ((abs(points[0][i].x - points[1][i].x) + abs(points[0][i].y - points[1][i].y)) > 2);
}






//-------------------------------------------------------------------------------------------------
//函数：InvSqrt
//功能：开平方
//参数：被开方数
//返回值：开方后的结果
//-------------------------------------------------------------------------------------------------


float InvSqrt(float x)
{
    float xhalf = 0.5f*x;
    int i = *(int*)&x;      // get bits for floating VALUE 
    i = 0x5f375a86- (i>>1); // gives initial guess y0
    x = *(float*)&i;        // convert bits BACK to float
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
    x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
 
    return 1/x;
}




//-------------------------------------------------------------------------------------------------
//函数：StatisticAngle
//功能：输出最大可能的角度值
//参数：*Angle
//返回值：likely_angle
//-------------------------------------------------------------------------------------------------

int StatisticAngle(double *angle)
{
	int i=0,j=0,likely_angle=0;
	int length[360]={0};
	for(i=-180;i<180;i++) {
		for(j=0;j<=sizeof(angle);j++) {
			if (abs((int)angle[j]-i)<10) length[i+180]++;
		}
		if(length[likely_angle]<=length[i+180]) likely_angle=i+180;
	}
	likely_angle-=180;
	if(likely_angle==9) likely_angle=0;
	return likely_angle;
}


float standardDeviation(double *data) {
	float mean=0.0, sum_deviation=0.0;
    int i,n;
	n = sizeof(data);
    for(i=0; i<n;++i)
    {
        mean+=data[i];
    }
    mean=mean/n;
    for(i=0; i<n;++i)
    sum_deviation+=(data[i]-mean)*(data[i]-mean);
    return sqrt(sum_deviation/n);          
}


/*
float mat_data(int *mat_x, int *mat_y) {
	int i=0,x_rel=0,y_rel=0;
	double distance=0,position=0;
	for(i=0;i<sizeof(mat_x);i++) {
		x_rel+=mat_x[i];
		y_rel+=mat_y[i];
	}
	if(x_rel!=0 && y_rel!=0) {
		distance = InvSqrt(x_rel*x_rel+y_rel*y_rel);
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
	if(distance<20 || distance>1000) position=0;
	printf("distance: %f\n",distance*0.2);
	return position;	
}*/
