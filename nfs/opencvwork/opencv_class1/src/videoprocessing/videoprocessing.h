/*------------------------------------------------------------------------------------------*\
   ������ȡÿһ֡ͼ�񣬲�ʹ��ͼ��������������
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

//������
void canny(cv::Mat& img, cv::Mat& out) {

   // ת��Ϊ�Ҷ�
   if (img.channels()==3)
      cv::cvtColor(img,out,CV_BGR2GRAY);
   // ����Canny��Ե
   cv::Canny(out,out,100,200);
   // ��תͼ��
   cv::threshold(out,out,128,255,cv::THRESH_BINARY_INV);
}

void salt(cv::Mat& image, cv::Mat& out) {             
	int i, j;
	for (int k=0; k<100; k++) {
		//read()�������������
		i = std::rand()%image.cols;
		j = std::rand()%image.rows;

		if (image.type() == CV_8UC1) {		//�Ҷ�ͼ��
			image.at<uchar>(j,i) = 255;
		} else if (image.type() == CV_8UC3) {		//��ɫͼ��
			image.at<cv::Vec3b>(j,i)[0] = 255;
			image.at<cv::Vec3b>(j,i)[1] = 255;
			image.at<cv::Vec3b>(j,i)[2] = 255;
		}

	}
}



int videoprocessing(void)
{
	// ����Ƶ�ļ�
	cv::VideoCapture capture(0);
//	cv::VideoCapture capture("http://www.laganiere.name/bike.avi");
	// ����Ƿ�ɹ�����Ƶ�ļ�
	if (!capture.isOpened())
		return 0;

	// ��ȡ��Ƶ֡��
	double rate= capture.get(CV_CAP_PROP_FPS);
	std::cout << "Frame rate: " << rate << "fps" << std::endl;

	bool stop(false);
	cv::Mat frame;				// ��ǰ��Ƶ֡
	cv::namedWindow("Extracted Frame");

	// ÿ֮֡�����ʱ
	//��Ӧ����Ƶ֡����
	//int delay= 1000/rate;
	int delay=30;
	long long i=0;
	std::string b="bike";
	std::string ext=".bmp";
	// Ϊ��Ƶ������֡
	while (!stop) {

		// read next frame if any
		if (!capture.read(frame))
			break;

		cv::imshow("Extra	cted Frame",frame);

		std::string name(b);
        // note: ĳЩMinGW���������ɴ����������
        // ���Ǽ������һ��bug
        // try: std::ostringstream ss; ss << i; name+= ss.rdbuf(); i++;
 		//name+=std::to_string(i++);
        std::ostringstream ss; ss << i; name+= ss.str(); i++;
		name+=ext;

		std::cout << name <<std::endl;
		
		cv::imwrite(name,frame);

		// ������ʱ
		// ���Ű���ֹͣ
		if (cv::waitKey(delay)>=0)
				stop= true;
	}

	// �ر���Ƶ�ļ�
	capture.release();

	cv::waitKey();

	// ����ʹ�� VideoProcessor ��

	//����ʵ��
	VideoProcessor processor;

	// ����Ƶ�ļ�
	processor.setInput(0);

	// ����һ����������ʾ��Ƶ
	processor.displayInput("Input Video");
	processor.displayOutput("Output Video");

	// ��ԭʼ��֡���ʲ�����Ƶ
	//processor.setDelay(1000./processor.getFrameRate());
	processor.setDelay(30);
	// ����֡�������Ļص�����
	processor.setFrameProcessor(canny);

	// ���һ����Ƶ
	processor.setOutput("��Ƶ���",-1,15);

	// ֹͣ����һ��ܵĽ���
	// processor.stopAtFrameNo(51);

	// ��������
	processor.run();

	cv::waitKey();	

	return 0;
}
