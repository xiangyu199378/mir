#include "SocketVideoserver.hpp"  

Socketclient socketVideo;  

Socketclient::Socketclient(void) {  }    
Socketclient::~Socketclient(void)  {  }   
 
int Socketclient::Connect(const char* IP, int PORT)  
{  
	struct sockaddr_in  servaddr;  
	if ((sockClient = socket(AF_INET, SOCK_STREAM, 0)) < 0)  {  
		printf("create socket error: %s(errno: %d)\n", strerror(errno), errno);  
		return -1;  
	}  

	memset(&servaddr, 0, sizeof(servaddr));  
	servaddr.sin_family = AF_INET;  
	servaddr.sin_port = htons(PORT); 
	 
	if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0)   {  
		printf("inet_pton error for %s\n", IP);  
		return -1;  
	}  

	if (connect(sockClient, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)   {  
		printf("connect error: %s(errno: %d)\n", strerror(errno), errno);  
		return -1;  
	}  
	else   { printf("connect successful!\n");  }  
}  


void Socketclient::Disconnect(void)  {  
   close(sockClient);  
}  

int Socketclient::transmit(Mat image)  
{  
	if (image.empty())  {  
	printf("empty image\n\n");  
	return -1;  
	}  

	if(image.cols != IMG_WIDTH || image.rows != IMG_HEIGHT || image.type() != CV_8UC3)  {  
	printf("the image must satisfy : cols == IMG_WIDTH（%d）  rows == IMG_HEIGHT（%d） type == CV_8UC3\n\n", IMG_WIDTH, IMG_HEIGHT);  
	return -1;  
	}  

	for(int k = 0; k < PACKAGE_NUM; k++)   {  
	    int num1 = IMG_HEIGHT / PACKAGE_NUM * k;  
		for (int i = 0; i < IMG_HEIGHT / PACKAGE_NUM; i++)  {  
			int num2 = i * IMG_WIDTH * 3;  
			uchar* ucdata = image.ptr<uchar>(i + num1);  
			for (int j = 0; j < IMG_WIDTH * 3; j++)  {  
				 data.buf[num2 + j] = ucdata[j];  
			}  
	    }  

		if(k == PACKAGE_NUM - 1)  
		data.flag = 2;  
		else  
		data.flag = 1;  

		if (send(sockClient, (char *)(&data), sizeof(data), 0) < 0)  {  
		printf("send image error: %s(errno: %d)\n", strerror(errno), errno);  
		return -1;  
		}  
	} 	 
}  

int Socketclient::test(void)
{
	if (socketVideo.Connect("192.168.1.152", 6666) < 0)  {  
	return 0;  
	}  

	VideoCapture capture(0);  
	Mat image;  

	while (1)  
	{  
	  if (!capture.isOpened())  return 0;  
	  capture >> image;  
	  if (image.empty())  return 0;  
	  socketVideo.transmit(image);  
	}  
	socketVideo.Disconnect();  
return 0;	  
}
