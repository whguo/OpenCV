#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

class Histogram1D
{
private:
	int histSize[1];         //项的数量
	float hranges[2];		//像素最大及最小值
	const float *ranges[1];	
	int channels[1];		//仅用到一个通道
public:
	Histogram1D()
	{
		histSize[0] = 256;
		hranges[0] = 0.0;
		hranges[1] = 255.0;
		ranges[0] = hranges;
		channels[0] = 0;
	}
	cv::MatND getHistogram(const cv::Mat &image)
	{
		cv::MatND hist;
		cv::calcHist(&image,
			1,				//单张图像
			channels,		//通道
			cv::Mat(),		//不使用图像作为掩码
			hist,			//返回的直方图
			1,				//1D的直方图
			histSize,		//项的数量
			ranges);		//像素值范围
		return hist;
	}

	cv::Mat getHistogramImage(const  cv::Mat &image)
	{
		cv::MatND hist = getHistogram(image);
		double maxVal = 0;
		double minVal = 0;
		cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);        //获取最大最小值
		cv::Mat histImg(histSize[0], histSize[0], CV_8U, cv::Scalar(255));
		int hpt = static_cast<int>(0.9*histSize[0]);		//设置最高点为nbins的90%
		for (int h = 0; h < histSize[0]; ++h)
		{
			float binVal = hist.at<float>(h);
			int intensity = static_cast<int>(binVal*hpt / maxVal);
			cv::line(histImg, cv::Point(h, histSize[0]),
							  cv::Point(h, histSize[0] - intensity),
							  cv::Scalar::all(0));
		}
		return histImg;
	}
};

int main()
{
	cv::Mat image = cv::imread("test.jpg", 0);
	Histogram1D h;
	cv::namedWindow("Histogram");
	cv::imshow("image",image);
	cv::imshow("Histogram", h.getHistogramImage(image));

	cv::Mat thresholded;
	cv::threshold(image, thresholded, 150, 255, cv::THRESH_BINARY);
	cv::imshow("binary image",thresholded);
	cv::waitKey(0);

	//直方图均衡化
	cv::Mat result;
	cv::equalizeHist(image, result);
	cv::imshow("result", result);
	cv::waitKey(0);

	return 0;
}

