#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#define TEST 4

void salt(cv::Mat &image, int n);
void colorReduce(const cv::Mat &image,cv::Mat &result, int div=64);
void colorQuickReduce(cv::Mat &image, int div = 64);
void colorReduceIterator(cv::Mat image,int div = 64);

int main()
{
	cv::Mat image;
	image = cv::imread("test.jpg");
	if (!image.data)
	{
		exit(1);
	}

#if TEST==1
	//给图像一些随机的点
	salt(image, 3000);

#elif TEST==2	
	//图像缩减
	cv::Mat imageClone = image.clone();
	colorReduce(image,imageClone);
	cv::namedWindow("Image");
	cv::imshow("Image", image);
	cv::imshow("ImageClone", imageClone);

#elif TEST==3
	//高效的缩减(快速遍历)
	colorQuickReduce(image);

#elif TEST==4	
	colorReduceIterator(image);
#endif
	
	cv::namedWindow("Image");
	cv::imshow("Image",image);
	cv::waitKey(0);
	return 0;
}

void salt(cv::Mat &image, int n)
{
	for (int k = 0; k < n; ++k)
	{
		int i = rand()%image.cols;
		int j = rand() % image.rows;
		if (image.channels() == 1)
		{
			image.at<uchar>(j, i) = 255;
		}
		else if (image.channels() == 3)
		{
			image.at<cv::Vec3b>(j, i)[0] = 255;
			image.at<cv::Vec3b>(j, i)[1] = 255;
			image.at<cv::Vec3b>(j, i)[2] = 255;
		}
	}
}

void colorReduce(const cv::Mat &image,cv::Mat &result, int div)
{
	int nl = image.rows;
	int nc = image.cols*image.channels();
	for (int j = 0; j < nl; ++j)
	{
		const uchar* data_in = image.ptr<uchar>(j);    //ptr返回第j行的首地址
		uchar* data_out = result.ptr<uchar>(j);
		for (int i = 0; i < nc; ++i)
		{
			//data[i] = data[i] / div*div + div / 2;
			uchar mask = 0xFF << 6;
		    data_out[i] = (data_in[i] & mask) + div / 2;
		}
	}
}

void colorQuickReduce(cv::Mat &image, int div)
{
	int nl = image.rows;
	int nc = image.cols*image.channels();
	if (image.isContinuous())
	{
		nc = nc*nl;
		nl = 1;
	}

	//可以替代以上代码
	//if (image.isContinuous())
	//{
	//	image.reshape(1, image.cols*image.rows);
	//}
	//int nl = image.rows;
	//int nc = image.cols;

	for (int j = 0; j < nl; ++j)
	{
		uchar* data = image.ptr<uchar>(j);
		for (int i = 0; i < nc; ++i)
		{
			data[i] = data[i] / div*div + div / 2;
		}
	}
}

void colorReduceIterator(cv::Mat image, int div)
{
	cv::Mat_<cv::Vec3b>::iterator it = image.begin<cv::Vec3b>();
	cv::Mat_<cv::Vec3b>::iterator itend = image.end<cv::Vec3b>();

	for (; it != itend; ++it)
	{
		(*it)[0] = (*it)[0] / div*div + div / 2;
		(*it)[1] = (*it)[1] / div*div + div / 2;
		(*it)[2] = (*it)[2] / div*div + div / 2;
	}
}