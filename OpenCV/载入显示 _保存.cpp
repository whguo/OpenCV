#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

int main()
{
	cv::Mat image;
	std::cout << "size:" << image.size().height << "," << image.size().width << std::endl;
	image = cv::imread("test.jpg");
	if (!image.data)
	{
		exit(1);
	}
	std::cout << "size:" << image.size().height << "," << image.size().width << std::endl;
	cv::namedWindow("Original Image");
	cv::imshow("Original Image", image);
	cv::Mat result;
	cv::flip(image, result, -1);
	cv::imshow("Output Image",result);
	//cv::Mat ima(480, 640, CV_8U, cv::Scalar(100));     ´´½¨»Ò¶ÈÍ¼
	cv::waitKey(0);
	return 0;
}