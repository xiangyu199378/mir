/*------------------------------------------------------------------------------------------*\

\*------------------------------------------------------------------------------------------*/

#if !defined FTRACKER
#define FTRACKER

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>

#include "../videoprocessing/videoprocessor.h"

class FeatureTracker : public FrameProcessor {

	
	
	cv::Mat gray;			// current gray-level image
	cv::Mat gray_prev;		// previous gray-level image
	std::vector<cv::Point2f> points[2]; // tracked features from 0->1
	std::vector<cv::Point2f> initial;   // initial position of tracked points
	std::vector<cv::Point2f> features;  // detected features
	int max_count;	  // maximum number of features to detect
	double qlevel;    // quality level for feature detection
	double minDist;   // minimum distance between two feature points
	std::vector<uchar> status; // status of tracked features
    std::vector<float> err;    // error in tracking

  public:

	FeatureTracker() : max_count(300), qlevel(0.01), minDist(10.) {}
	
	// processing method
	void process(cv:: Mat &frame, cv:: Mat &output) {

		// convert to gray-level image
		cv::cvtColor(frame, gray, CV_BGR2GRAY); 
		frame.copyTo(output);

		// 1. if new feature points must be added
		if(addNewPoints())
		{
			// detect feature points
			detectFeaturePoints();
			// add the detected features to the currently tracked features
			points[0].insert(points[0].end(),features.begin(),features.end());
			initial.insert(initial.end(),features.begin(),features.end());
		}
		
		// for first image of the sequence
		if(gray_prev.empty())
           gray.copyTo(gray_prev);
            
		// 2. L-K光流法运动估计
		cv::calcOpticalFlowPyrLK(gray_prev, gray, // 2 consecutive images
			points[0], // input point position in first image
            points[1], // output point postion in the second image
                status,    // tracking success
                err);      // tracking error

            // 3. 遍历提取不好的特征点
            int k=0;
            for( int i= 0; i < points[1].size(); i++ ) {

                // do we keep this point?
                if (acceptTrackedPoint(i)) {

                    // keep this point in vector
                    initial[k]= initial[i];
                    points[1][k++] = points[1][i];
                }
            }

		// 去除不好的特征点
        points[1].resize(k);
		initial.resize(k);

        // 4. 处理被跟踪的特征点
		handleTrackedPoints(frame, output);

        // 5. 把当前跟踪结果作为下一此参考
		std::swap(points[1], points[0]);
        cv::swap(gray_prev, gray);
	}

	// feature point detection
	void detectFeaturePoints() {
			
		// detect the features
		cv::goodFeaturesToTrack(gray, // the image 
			features,   // the output detected features
			max_count,  // the maximum number of features 
			qlevel,     // quality level
			minDist);   // min distance between two features
	}

	// determine if new points should be added
	bool addNewPoints() {

		// if too few points
		return points[0].size()<=10;
	}

	// determine which tracked point should be accepted
	// here we keep only moving points
	bool acceptTrackedPoint(int i) {

		return status[i] &&
			// if point has moved
			(abs(points[0][i].x-points[1][i].x)+
			(abs(points[0][i].y-points[1][i].y))>2);
	}

	// handle the currently tracked points
	void handleTrackedPoints(cv:: Mat &frame, cv:: Mat &output) {
		double position=0;           //运动位置状态
		int x_rel,y_rel;
		double angle[400];

		// for all tracked points
		for(int i= 0; i < points[1].size(); i++ ) {

			// draw line and circle
		    cv::line(output, initial[i], points[1][i], cv::Scalar(255,255,255));
			cv::circle(output, points[1][i], 3, cv::Scalar(255,255,255),-1);
			x_rel = points[0][i].x-points[1][i].x;
			y_rel = points[0][i].y-points[1][i].y;
			//printf("Now_point(%f,%f)\r\n",initial[i].x,initial[i].y);           //输出特征点坐标
			if(x_rel!=0 && y_rel!=0) {
				position = y_rel/(InvSqrt(x_rel*x_rel+y_rel*y_rel));
				position = (asin(position)*180)/CV_PI;
			}
			//printf("X is :%d,Y is :%d\n",x,y);
		
			angle[i] = position;                   //将获得的角度存入数组
			
		}
		printf("%d\n",StatisticAngle(angle));
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
		int i,j,likely_angle=0;
		int length[360]={0};
		for(i=-180;i<180;i++) {
			for(j=0;j<=300;j++) {
				if ((int)angle[j]==i) length[i+180]++;
			}
			if(length[likely_angle]<=length[i+180]) likely_angle=i+180;
		}
		likely_angle-=180;
		return likely_angle;
	}

	
};

#endif
