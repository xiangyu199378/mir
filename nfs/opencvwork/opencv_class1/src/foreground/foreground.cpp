/*------------------------------------------------------------------------------------------*\
								提取并输出前景
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

#include "../videoprocessing/videoprocessor.h"
#include "BGFGSegmentor.h"
int foreground(void)
{
	// Open the video file
    cv::VideoCapture capture(0);
	// check if video successfully opened
	if (!capture.isOpened())
		return 0;

	// current video frame
	cv::Mat frame; 
	// foreground binary image
	cv::Mat foreground;
	// background image
	cv::Mat background;

	cv::namedWindow("Extracted Foreground");

	// The Mixture of Gaussian object
	// used with all default parameters
	cv::BackgroundSubtractorMOG mog;

	bool stop(false);
	// for all frames in video
	while (!stop) {

		// read next frame if any
		if (!capture.read(frame))
			break;

		// update the background
		// and return the foreground
		mog(frame,foreground,0.01);

		// Complement the image
		cv::threshold(foreground,foreground,128,255,cv::THRESH_BINARY_INV);

		// show foreground and background
		cv::imshow("Extracted Foreground",foreground);

		// introduce a delay
		// or press key to stop
		if (cv::waitKey(10)>=0)
				stop= true;
	}

	cv::waitKey();

	// Create video procesor instance
	VideoProcessor processor;

	// Create background/foreground segmentor 
	BGFGSegmentor segmentor;
	segmentor.setThreshold(25);

	// Open video file
	processor.setInput(0);

	// set frame processor
	processor.setFrameProcessor(&segmentor);

	// Declare a window to display the video
	processor.displayOutput("Extracted Foreground");

	// Play the video at the original frame rate
	//processor.setDelay(1000./processor.getFrameRate());
	processor.setDelay(100);

	// Start the process
	processor.run();

	cv::waitKey();
}
