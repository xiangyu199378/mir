/*------------------------------------------------------------------------------------------*\
								¹âÁ÷·¨×·×Ù
\*------------------------------------------------------------------------------------------*/

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/video/tracking.hpp>

#include "featuretracker.h"

void tracking(void)
{
	// Create video procesor instance
	VideoProcessor processor;

	// Create feature tracker instance
	FeatureTracker tracker;

	// Open video file
	processor.setInput(0);

	// set frame processor
	processor.setFrameProcessor(&tracker);

	// Declare a window to display the video
	processor.displayOutput("Tracked Features");

	// Play the video at the original frame rate
	//processor.setDelay(1000./processor.getFrameRate());
	processor.setDelay(30);
	processor.stopAtFrameNo(90);

	// Start the process
	processor.run();

	cv::waitKey();
}
