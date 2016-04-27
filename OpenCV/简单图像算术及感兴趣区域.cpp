#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define TEST 4

int main()
{
	cv::Mat image1,image2,image3;
	image1 = cv::imread("test.jpg");
	image2 = cv::imread("test2.jpg");
	image3 = cv::imread("test3.jpg");
	if (!image1.data && !image2.data && !image3.data)
	{
		exit(1);
	}

#if TEST==1
	//利用函数加
	cv::Mat result;
	cv::addWeighted(image1, 0.9, image2, 0.7,0, result);
	cv::namedWindow("Image");
	cv::imshow("Image",result);

#elif TEST==2	
	//重载图像操作符
	cv::Mat result;
	result = 0.9*image1 + 0.7*image2;
	cv::namedWindow("Image");
	cv::imshow("Image", result);

#elif TEST==3
	//感兴趣区域
	cv::Mat imageROI;
	imageROI = image1(cv::Rect(500, 400, image3.cols, image3.rows));
	cv::addWeighted(imageROI,1,image3,0.3,0,imageROI);
	cv::namedWindow("Image");
	cv::imshow("Image", image1);

#elif TEST==4	
	//感兴趣区域掩模
	cv::Mat imageROI;
	imageROI = image1(cv::Rect(560, 400, image3.cols, image3.rows));
	cv::Mat mask = cv::imread("test3.jpg",0);
	image3.copyTo(imageROI, mask);
	cv::namedWindow("Image");
	cv::imshow("Image", image1);
#endif
	
	cv::waitKey(0);
	return 0;
}
