#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define TEST 4

int main()
{
	cv::Mat image = cv::imread("test.jpg",0);
	cv::namedWindow("image");
	cv::threshold(image,image,180,255,cv::THRESH_BINARY);
	cv::imshow("image", image);
#if TEST == 1
	//默认的结构元素腐蚀与膨胀
	cv::Mat eroded;
	cv::erode(image,eroded,cv::Mat());
	cv::imshow("erode", eroded);
	cv::Mat dilated;
	cv::dilate(image, dilated, cv::Mat());
	cv::imshow("dilated", dilated);
#elif TEST==2
	//自定义结构元素
	cv::Mat element(7,7,CV_8U,cv::Scalar(1));
	cv::Mat eroded;
	cv::erode(image,eroded,element);
	cv::imshow("erode", eroded);
#elif TEST==3
	//多次腐蚀
	cv::Mat eroded;
	cv::erode(image, eroded, cv::Mat(),cv::Point(-1,-1),3);
	cv::imshow("erode", eroded);
#elif TEST==4
	//闭运算：先膨胀再腐蚀
	cv::Mat element(5,5,CV_8U,cv::Scalar(1));
	cv::Mat closed;
	cv::morphologyEx(image,closed,cv::MORPH_CLOSE,element);
	cv::imshow("closed",closed);
	//开运算：先腐蚀再膨胀
	cv::Mat opened;
	cv::morphologyEx(image, opened, cv::MORPH_OPEN, element);
	cv::imshow("opened", opened);
#endif
	cv::waitKey(0);
	return 0;

}

