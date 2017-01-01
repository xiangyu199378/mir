/*------------------------------------------------------------------------------------------*\
   用于提取每一帧图像，并使用图像处理函数处理后输出
\*------------------------------------------------------------------------------------------*/

#include <string>
#include <iostream>
#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "videoprocessor.h"

void draw(const cv::Mat& img, cv::Mat& out) {

	img.copyTo(out);
	cv::circle(out, cv::Point(100,100),5,cv::Scalar(255,0,0),2);
}

//处理函数
void canny(cv::Mat& img, cv::Mat& out) {

   // 转换为灰度
   if (img.channels()==3)
      cv::cvtColor(img,out,CV_BGR2GRAY);
   // 计算Canny边缘
   cv::Canny(out,out,100,200);
   // 反转图像
   cv::threshold(out,out,128,255,cv::THRESH_BINARY_INV);
}

void salt(cv::Mat& image, cv::Mat& out) {             
	int i, j;
	for (int k=0; k<100; k++) {
		//read()是随机数生成器
		i = std::rand()%image.cols;
		j = std::rand()%image.rows;

		if (image.type() == CV_8UC1) {		//灰度图像
			image.at<uchar>(j,i) = 255;
		} else if (image.type() == CV_8UC3) {		//彩色图像
			image.at<cv::Vec3b>(j,i)[0] = 255;
			image.at<cv::Vec3b>(j,i)[1] = 255;
			image.at<cv::Vec3b>(j,i)[2] = 255;
		}

	}
}



int videoprocessing(void)
{
	// 打开视频文件
	cv::VideoCapture capture(0);
//	cv::VideoCapture capture("http://www.laganiere.name/bike.avi");
	// 检查是否成功打开视频文件
	if (!capture.isOpened())
		return 0;

	// 获取视频帧率
	double rate= capture.get(CV_CAP_PROP_FPS);
	std::cout << "Frame rate: " << rate << "fps" << std::endl;

	bool stop(false);
	cv::Mat frame;				// 当前视频帧
	cv::namedWindow("Extracted Frame");

	// 每帧之间的延时
	//对应于视频帧速率
	//int delay= 1000/rate;
	int delay=30;
	long long i=0;
	std::string b="bike";
	std::string ext=".bmp";
	// 为视频中所有帧
	while (!stop) {

		// read next frame if any
		if (!capture.read(frame))
			break;

		cv::imshow("Extra	cted Frame",frame);

		std::string name(b);
        // note: 某些MinGW编译器生成错误的这条线
        // 这是计算机的一个bug
        // try: std::ostringstream ss; ss << i; name+= ss.rdbuf(); i++;
 		//name+=std::to_string(i++);
        std::ostringstream ss; ss << i; name+= ss.str(); i++;
		name+=ext;

		std::cout << name <<std::endl;
		
		cv::imwrite(name,frame);

		// 引入延时
		// 或着按键停止
		if (cv::waitKey(delay)>=0)
				stop= true;
	}

	// 关闭视频文件
	capture.release();

	cv::waitKey();

	// 现在使用 VideoProcessor 类

	//创建实例
	VideoProcessor processor;

	// 打开视频文件
	processor.setInput(0);

	// 声明一个窗口以显示视频
	processor.displayInput("Input Video");
	processor.displayOutput("Output Video");

	// 在原始的帧速率播放视频
	//processor.setDelay(1000./processor.getFrameRate());
	processor.setDelay(30);
	// 设置帧处理器的回调函数
	processor.setFrameProcessor(canny);

	// 输出一个视频
	processor.setOutput("视频输出",-1,15);

	// 停止在这一框架的进程
	// processor.stopAtFrameNo(51);

	// 启动进程
	processor.run();

	cv::waitKey();	

	return 0;
}
