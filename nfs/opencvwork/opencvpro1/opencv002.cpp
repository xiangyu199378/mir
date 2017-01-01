#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include <stdio.h>




using namespace cv;


#define  thresholdvalue 200

/*
---------------------------------------------------------------
---------------------------------------------------------------
*/
int channels, nrows, ncols;
bool loopcnt=true;
uchar table[256]; 

Mat g_srcImage, g_grayImage, g_dstImage;


Mat ChangeImg(Mat &img, const uchar* table);


static void showhelptxt(void)
{
    printf( "\n go! go! you are so good! \n");

}

int main( int argc, char** argv )
{

  CvCapture* capture = cvCreateCameraCapture(0);
  (void)argc,(void *)argv;
  showhelptxt();

  if( !capture ) { printf("drives is null \n"); return -1;}

  for (int i = 0; i < 256; i++){ if(i<thresholdvalue)table[i] = 0; else table[i]=255;}


 
 while(1)
  {
  
      g_srcImage = cvQueryFrame(capture);
      
     
    cvtColor(g_srcImage,g_grayImage, CV_BGR2GRAY);
 //   Canny(g_grayImage, g_dstImage,50,200,3);
  //   g_dstImage = ChangeImg(g_grayImage, table);
  //   imshow("oneday",  g_srcImage);  
//      imshow("oneday", g_grayImage); 
      if(cvWaitKey(30)>0) break;
//      if(!capture.isOpened())  break;
      
   }
   cvReleaseCapture(&capture);

   return (0);
}


void set_reMapValue(int Threshold, uchar* table)
{
   for (int i = 0; i < 256; i++){ table[i] = i / 100 * 100; }
}


Mat ChangeImg(Mat &img, const uchar* table)
{      
	  if (loopcnt)
       {
		channels = g_srcImage.channels();
		nrows    = g_srcImage.rows;                    
		ncols    = g_srcImage.cols*channels;            

		loopcnt = false;

		printf("picture cols:%d rows:%d\n", g_srcImage.cols, g_srcImage.rows);
		printf("picture is continuous\n");
	}
     
     if (img.isContinuous())
	{
		ncols *= nrows;
		nrows  = 1;
	}

	for (int i = 0; i<nrows; i++)
	{
		uchar *p = img.ptr<uchar>(i);   
		for (int j = 0; j<ncols; j++)
		{
			p[j] = table[p[j]];        
		}
	}

 return img;
}





