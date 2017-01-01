/*------------------------------------------------------------------------------------------*\
									自定义帧处理器
\*------------------------------------------------------------------------------------------*/

#if !defined VPROCESSOR
#define VPROCESSOR

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

// 帧处理器的接口
class FrameProcessor {

  public:
	// 处理方法
	virtual void process(cv:: Mat &input, cv:: Mat &output)= 0;
};

class VideoProcessor {

  private:

	  // OpenCV视频捕获对象
	  cv::VideoCapture capture;
	  // 处理每一帧时都会调用的回调函数
	  void (*process)(cv::Mat&, cv::Mat&);
	  // the pointer to the class implementing 
	  // the FrameProcessor interface
	  FrameProcessor *frameProcessor;
	  // 布尔型变量，表示该回调函数是否会被调用
	  bool callIt;
	  // 输入窗口的显示名称
	  std::string windowNameInput;
	  // 输出窗口的显示名称
	  std::string windowNameOutput;
	  // 帧之间的延时
	  int delay;
	  // 已经处理的帧数
	  long fnumber;
	  // 达到这个帧数时结束处理
	  long frameToStop;
	  // 结束处理
	  bool stop;

	  // 图像文件名称的矢量被用来作为输入
	  std::vector<std::string> images; 
	  // 图像矢量迭代器
	  std::vector<std::string>::const_iterator itImg;

	  //Opencv视频写入对象
	  cv::VideoWriter writer;
	  // 输出名称
	  std::string outputFile;

	  // 输出图像的当前索引
	  int currentIndex;
	  // 输入图像文件名中的数字个数
	  int digits;
	  // 输出图像的延伸
	  std::string extension;

	  //捕获下一帧图像
	  // could be: video file; camera; vector of images
	  bool readNextFrame(cv::Mat& frame) {

		  if (images.size()==0)
			  return capture.read(frame);
		  else {

			  if (itImg != images.end()) {

				  frame= cv::imread(*itImg);
				  itImg++;
				  return frame.data != 0;
			  }

              return false;
		  }
	  }

	  // 写入输出框架
	  // could be: 视频文件或者图像
	  void writeNextFrame(cv::Mat& frame) {

		  if (extension.length()) { // 然后写入图像
		  
			  std::stringstream ss;
		      ss << outputFile << std::setfill('0') << std::setw(digits) << currentIndex++ << extension;
			  cv::imwrite(ss.str(),frame);

		  } else { // 写入框架

			  writer.write(frame);
		  }
	  }

  public:

	  // 构造函数设置的默认值
	  VideoProcessor() : callIt(false), delay(-1), 
		  fnumber(0), stop(false), digits(0), frameToStop(-1), 
	      process(0), frameProcessor(0) {}

	  // 设置视频名称
	  bool setInput(std::string filename) {

		fnumber= 0;
		// 资源已与等实例相关联的案件
		capture.release();
		images.clear();

		// 打开视频文件
		return capture.open(filename);
	  }

	  // 设置相机的ID
	  bool setInput(int id) {

		fnumber= 0;
		// 已经是一种资源的情况下与等实例相关联
		capture.release();
		images.clear();

		// 打开视频文件
		return capture.open(id);
	  }

	  // 设置输入图像的矢量
	  bool setInput(const std::vector<std::string>& imgs) {

		fnumber= 0;
		// 已经是一种资源的情况下与等实例相关联
		capture.release();

		// 输入将会成为图像的矢量
		images= imgs;
		itImg= images.begin();

		return true;
	  }

	  // 设置输出视频文件，由默认相同的参数比将使用输入视频
	  bool setOutput(const std::string &filename, int codec=0, double framerate=0.0, bool isColor=true) {

		  outputFile= filename;
		  extension.clear();
		  
		  if (framerate==0.0) 
			  framerate= getFrameRate(); // same as input

		  char c[4];
		  //使用同一编码解码器作为输入
		  if (codec==0) { 
			  codec= getCodec(c);
		  }

		  // 打开输出视频
		  return writer.open(outputFile, // filename
			  codec, // 编码器将被使用
			  framerate,      // 视频的帧率
			  getFrameSize(), // 帧的大小
			  isColor);       // 彩色视频？
	  }

	  //设置输出为一系列图像文件，扩展名必须是.jpg,.bmp.....
	  bool setOutput(const std::string &filename, // 文件名前缀
		  const std::string &ext, // 图像文件扩展名
		  int numberOfDigits=3,   // 数字位数
		  int startIndex=0) {     //起始索引

		  // 数字的位数必须为正数
		  if (numberOfDigits<0)
			  return false;

		  // 文件名和他们共同的扩展
		  outputFile= filename;
		  extension= ext;

		  // 文件编号方案中的数字个数
		  digits= numberOfDigits;
		  // 开始在此索引编号
		  currentIndex= startIndex;

		  return true;
	  }

	  // 设置将为每个帧调用的回调函数
	  void setFrameProcessor(void (*frameProcessingCallback)(cv::Mat&, cv::Mat&)) {

		  // 使无效帧处理器类实例
		  frameProcessor= 0;
		  // 这是将调用帧处理器功能
		  process= frameProcessingCallback;
		  callProcess();
	  }

	  // set the instance of the class that implements the FrameProcessor interface
	  void setFrameProcessor(FrameProcessor* frameProcessorPtr) {

		  // 无效的回调函数
		  process= 0;
		  // 这是将调用的帧处理器实例
		  frameProcessor= frameProcessorPtr;
		  callProcess();
	  }

	  // 停留在此帧数
	  void stopAtFrameNo(long frame) {

		  frameToStop= frame;
	  }

	  //进程将被回调
	  void callProcess() {

		  callIt= true;
	  }

	  // 不使用进程回调
	  void dontCallProcess() {

		  callIt= false;
	  }

	  // 显示输入帧
	  void displayInput(std::string wn) {
	    
		  windowNameInput= wn;
		  cv::namedWindow(windowNameInput);
	  }

	  // 显示已处理的帧
	  void displayOutput(std::string wn) {
	    
		  windowNameOutput= wn;
		  cv::namedWindow(windowNameOutput);
	  }

	  // 不显示已处理的帧
	  void dontDisplay() {

		  cv::destroyWindow(windowNameInput);
		  cv::destroyWindow(windowNameOutput);
		  windowNameInput.clear();
		  windowNameOutput.clear();
	  }

	  // 设置帧之间的延时
	  // 0 表示每一帧都要等待
	  // 负数表示不延时
	  void setDelay(int d) {
	  
		  delay= d;
	  }

	  // 保存处理帧的计数
	  long getNumberOfProcessedFrames() {
	  
		  return fnumber;
	  }

	  // 返回视频帧的大小
	  cv::Size getFrameSize() {

		if (images.size()==0) {

			// 获得来自捕获设备的大小
			int w= static_cast<int>(capture.get(CV_CAP_PROP_FRAME_WIDTH));
			int h= static_cast<int>(capture.get(CV_CAP_PROP_FRAME_HEIGHT));

			return cv::Size(w,h);

		} else { // 如果输入时矢量图像

			cv::Mat tmp= cv::imread(images[0]);
			if (!tmp.data) return cv::Size(0,0);
			else return tmp.size();
		}
	  }

	  //返回下一帧的帧数
	  long getFrameNumber() {

		if (images.size()==0) {

			// 从捕获设备获取的信息
	 	    long f= static_cast<long>(capture.get(CV_CAP_PROP_POS_FRAMES));
		    return f; 

		} else { // 如果输入矢量图像

			return static_cast<long>(itImg-images.begin());
		}
	  }

	  // 毫秒内返回位置
	  double getPositionMS() {

		  // 没有定义矢量图像
		  if (images.size()!=0) return 0.0;

	 	  double t= capture.get(CV_CAP_PROP_POS_MSEC);
		  return t; 
	  }

	  // 返回帧率
	  double getFrameRate() {

		  // 没有定义矢量图像
		  if (images.size()!=0) return 0;

	 	  double r= capture.get(CV_CAP_PROP_FPS);
		  return r; 
	  }

	  // 返回视频中的帧的编号
	  long getTotalFrameCount() {

		  // 图像的矢量
		  if (images.size()!=0) return images.size();

	 	  long t= capture.get(CV_CAP_PROP_FRAME_COUNT);
		  return t; 
	  }

	  // 获取视频的编解码器的输入
	  int getCodec(char codec[4]) {

		  // undefined for vector of images
		  if (images.size()!=0) return -1;

		  union {
			  int value;
			  char code[4]; } returned;

		  returned.value= static_cast<int>(capture.get(CV_CAP_PROP_FOURCC));

		  codec[0]= returned.code[0];
		  codec[1]= returned.code[1];
		  codec[2]= returned.code[2];
		  codec[3]= returned.code[3];

		  return returned.value;
	  }
	  
	  // 转到此帧数
	  bool setFrameNumber(long pos) {

		  // for vector of images
		  if (images.size()!=0) {

			  // move to position in vector
			  itImg= images.begin() + pos;
			  // is it a valid position?
			  if (pos < images.size())
				  return true;
			  else
				  return false;

		  } else { // 如果输入的是捕获设备

			return capture.set(CV_CAP_PROP_POS_FRAMES, pos);
		  }
	  }

	  // 转到位置
	  bool setPositionMS(double pos) {

		  // not defined in vector of images
		  if (images.size()!=0) 
			  return false;
		  else 
		      return capture.set(CV_CAP_PROP_POS_MSEC, pos);
	  }

	  // 转到此位置表示分数总视频长度
	  bool setRelativePosition(double pos) {

		  // for vector of images
		  if (images.size()!=0) {

			  // 移动向量中的位置
			  long posI= static_cast<long>(pos*images.size()+0.5);
			  itImg= images.begin() + posI;
			  // 他是否是有效位置
			  if (posI < images.size())
				  return true;
			  else
				  return false;

		  } else { // 如果输入是捕获设备

			  return capture.set(CV_CAP_PROP_POS_AVI_RATIO, pos);
		  }
	  }

	  // 停止处理
	  void stopIt() {

		  stop= true;
	  }

	  // 进程是否停止
	  bool isStopped() {

		  return stop;
	  }

	  // 捕获设备是否打开
	  bool isOpened() {

		  return capture.isOpened() || !images.empty();
	  }
	  
	  //抓取帧序列
	  void run() {

		  // 当前帧
		  cv::Mat frame;
		  // 输入帧
		  cv::Mat output;

		  // 没有已设置的捕获设备
		  if (!isOpened())
			  return;

		  stop= false;

		  while (!isStopped()) {

			  // 如果有的话读取下一帧
			  if (!readNextFrame(frame))
				  break;

			  // 显示输入帧
			  if (windowNameInput.length()!=0) 
				  cv::imshow(windowNameInput,frame);

		      // 调用过程函数或方法
			  if (callIt) {
				  
				// 过程框架
				if (process)
				    process(frame, output);
				else if (frameProcessor) 
					frameProcessor->process(frame,output);
				// 帧数递增
			    fnumber++;

			  } else {

				output= frame;
			  }

			  // 写入输出序列
			  if (outputFile.length()!=0)
				  writeNextFrame(output);

			  // 显示输出序列
			  if (windowNameOutput.length()!=0) 
				  cv::imshow(windowNameOutput,output);
			
			  // 引入延时
			  if (delay>=0 && cv::waitKey(delay)>=0)
				stopIt();

			  // 检测是否应停止
			  if (frameToStop>=0 && getFrameNumber()==frameToStop)
				  stopIt();
		  }
	  }
};

#endif
