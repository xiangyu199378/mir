


#include "featuretracking.hpp"


mir_tracking track;

mir_tracking:: mir_tracking(){}
mir_tracking::~mir_tracking(){}

static Mat image,dst;
static Point origin;
static Rect selection;

static bool selectObject = false ;
static int trackObject = 0 ;

 static void onMouse(int event, int x, int y, int, void*)
{
	if (selectObject)
	{
		selection.x = MIN(x, origin.x);
		selection.y = MIN(y, origin.y);
		selection.width = std::abs(x - origin.x);
		selection.height = std::abs(y - origin.y);

		selection &= Rect(0, 0, image.cols, image.rows);
	}
	switch (event)
	{
		
	case EVENT_LBUTTONDOWN:
		origin = Point(x, y);
		selection = Rect(x, y, 0, 0);
		selectObject = true;
		break;
	
	case EVENT_LBUTTONUP:
		selectObject = false;
		if (selection.width > 0 && selection.height > 0)
		{
			dst = image(Rect(selection.x, selection.y, selection.width, selection.height));
	//		namedWindow("dst");
	//		imshow("dst", dst);
			imwrite("saveImage.jpg", dst);
			trackObject = -1;
		}
		break;
	}
}

int featurePoint_x=0;
int featurePoint_y=0;
//---------------------------------------------------ORB算法---------------------------------------------------//
int mir_tracking::featurePicture(Mat img_1 ,Mat img_2) {
	float k1=0,k2=0,b1=0,b2=0;
	//Mat img_1 = imread("/home/fa/opencvwork/opencvdream2/saveImage.jpg");			//在这里读取文件必须使用绝对路径
	
    if (!img_1.data || !img_2.data)  
    {  
        cout << "error reading images " << endl;  
        return -1;  
    }  
  
    ORB orb;  
    vector<KeyPoint> keyPoints_1, keyPoints_2;  
    Mat descriptors_1, descriptors_2;


	orb(img_1, Mat(), keyPoints_1, descriptors_1); 
    orb(img_2, Mat(), keyPoints_2, descriptors_2);

    BruteForceMatcher<HammingLUT> matcher;  
    vector<DMatch> matches;  
    matcher.match(descriptors_1, descriptors_2, matches);  
  
    double max_dist = 0; double min_dist = 100;  
    //-- Quick calculation of max and min distances between keypoints
	for( int i = 0; i < descriptors_1.rows; i++ )  
	{   
		double dist = matches[i].distance;  
		if( dist < min_dist ) min_dist = dist;  
		if( dist > max_dist ) max_dist = dist;  
	}  
	//printf("-- Max dist : %f \n", max_dist );  
	//printf("-- Min dist : %f \n", min_dist );  
	
	//-- Draw only "good" matches (i.e. whose distance is less than 0.6*max_dist )   
	//-- PS.- radiusMatch can also be used here.   
	std::vector< DMatch > good_matches;  
	for( int i = 0; i < descriptors_1.rows; i++ )  
	{   
		if( matches[i].distance < 0.6*max_dist )  
		{   
			good_matches.push_back( matches[i]);   
		}  
	}  
	
	Mat img_matches;  
	drawMatches(img_1, keyPoints_1, img_2, keyPoints_2,  
	good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),  
	vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);  
	
	// localize the object   
	std::vector<Point2f> obj;  
	std::vector<Point2f> scene;  
	for (size_t i = 0; i < good_matches.size(); ++i)  
	{  
		// 在优秀的匹配中获取特征点   
		obj.push_back(keyPoints_1[ good_matches[i].queryIdx ].pt);  
		scene.push_back(keyPoints_2[ good_matches[i].trainIdx ].pt);  
	} 
	if (scene.size() >= 4) {			//必须>=4,CV_Assert函数的特点
		Mat H = findHomography( obj, scene, CV_RANSAC );  
  
		//获取视频中边缘的特征点   
		std::vector<Point2f> obj_corners(4);  
		obj_corners[0] = cvPoint(0,0);  
		obj_corners[1] = cvPoint( img_1.cols, 0);  
		obj_corners[2] = cvPoint( img_1.cols, img_1.rows);  
		obj_corners[3] = cvPoint( 0, img_1.rows);  
		std::vector<Point2f> scene_corners(4);  
		perspectiveTransform( obj_corners, scene_corners, H);  
  
		// draw lines between the corners (the mapped object in the scene - image_2)   
		line( img_matches, scene_corners[0] + Point2f( img_1.cols, 0), scene_corners[1] + Point2f( img_1.cols, 0),Scalar(0,255,0));  
		line( img_matches, scene_corners[1] + Point2f( img_1.cols, 0), scene_corners[2] + Point2f( img_1.cols, 0),Scalar(0,255,0));  
		line( img_matches, scene_corners[2] + Point2f( img_1.cols, 0), scene_corners[3] + Point2f( img_1.cols, 0),Scalar(0,255,0));  
		line( img_matches, scene_corners[3] + Point2f( img_1.cols, 0), scene_corners[0] + Point2f( img_1.cols, 0),Scalar(0,255,0));
		if((scene_corners[0].x-scene_corners[2].x)!=0 && (scene_corners[1].x-scene_corners[3].x)!=0) {
			k1=(scene_corners[0].y-scene_corners[2].y)/(scene_corners[0].x-scene_corners[2].x);
			k2=(scene_corners[1].y-scene_corners[3].y)/(scene_corners[1].x-scene_corners[3].x);
		}
		b1=scene_corners[0].y-k1*scene_corners[0].x;
		b2=scene_corners[1].y-k2*scene_corners[1].x;
		if(k1!=k2) {
			featurePoint_x=(b2-b1)/(k1-k2);
			featurePoint_y=k1*featurePoint_x+b1;
		}
		//featurePoint_x=(scene_corners[0].x+scene_corners[2].x)/2;
		//featurePoint_y=(scene_corners[0].y+scene_corners[2].y)/2;
		printf("%d,%d\n",featurePoint_x,featurePoint_y);
	}
	 
	imshow( "Match", img_matches);  
	
	waitKey(3); 
    return 0;  
}

int mir_tracking::getORB(  Mat &img_1 ,Mat &img_2 )
{

   double t;
   t=getTickCount();
    img_1 = imread("saveImage.jpg");

   if (!img_1.data || !img_2.data)   {
      cout << "error reading images " << endl;      return -1;
   }
   
   ORB orb;
   vector<KeyPoint> keyPoints_1, keyPoints_2;
   Mat descriptors_1, descriptors_2;
   
   orb(img_1, Mat(), keyPoints_1, descriptors_1);
   orb(img_2, Mat(), keyPoints_2, descriptors_2);
   BruteForceMatcher<HammingLUT> matcher;
   vector<DMatch> matches;
   matcher.match(descriptors_1, descriptors_2, matches);
   
   double max_dist = 0; double min_dist = 100;
   for( int i = 0; i < descriptors_1.rows; i++ )   {
      double dist = matches[i].distance;
      if( dist < min_dist ) min_dist = dist;
      if( dist > max_dist ) max_dist = dist;
   }
   printf("-- Max dist : %f \n", max_dist );
   printf("-- Min dist : %f \n", min_dist );
   
   std::vector< DMatch > good_matches;
   for( int i = 0; i < descriptors_1.rows; i++ )   {
     if( matches[i].distance < 0.6*max_dist ){
      good_matches.push_back(matches[i]);
      }
   }
  t=getTickCount()-t;
  t=t*1000/getTickFrequency();
  Mat img_matches;
  drawMatches(img_1, keyPoints_1, img_2, keyPoints_2,good_matches, img_matches,
   Scalar::all(-1), Scalar::all(-1),vector<char>(),
   DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
  imshow( "Match", img_matches);   
  printf( "%f ms\n", t );
 // cvWaitKey(0);
  return 0;
}
int mir_tracking::test_org(void)
{
	VideoCapture cap;
	Rect trackWindow;
	int hsize = 16;
	float hranges[] = { 0, 180 };
	const float* phranges = hranges;

	cap.open(0);


	namedWindow("CamShift Demo", 0);
	setMouseCallback("CamShift Demo", onMouse, 0);

	Mat frame;
//	, hsv, hue, mask, hist, histimg = Mat::zeros(200, 320, CV_8UC3), backproj;

	bool paused = false;

	while (1)
	{
		if (!paused)
		{
			cap >> frame;
			if (frame.empty())
				break;
		}

		frame.copyTo(image);
/*
		if (selectObject && selection.width > 0 && selection.height > 0)
		{
			Mat roi(image, selection);
			bitwise_not(roi, roi);
		}
*/

	imshow("CamShift Demo", image);
	
	   if(trackObject ==-1)
	     track.featurePicture(dst,image);
       //   track.getORB(dst,image) ;
          
          
		waitKey(10);
	}

	return 0;
}

