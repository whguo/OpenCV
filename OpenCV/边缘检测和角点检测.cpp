#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

class MorphoFeatures
{
private:
	int threshold;
	cv::Mat cross;
	cv::Mat diamond;
	cv::Mat square;
	cv::Mat x;
public:
	MorphoFeatures():threshold(-1), cross(5, 5, CV_8U, cv::Scalar(0)),
		diamond(5, 5, CV_8U, cv::Scalar(1)), square(5, 5,CV_8U, cv::Scalar(1)), x(5,5,CV_8U,cv::Scalar(0))
	{
		//ʮ����Ԫ��
		for (int i = 0; i < 5; ++i)
		{
			cross.at<uchar>(2, i) = 1;
			cross.at<uchar>(i, 2) = 1;
		}
		//����Ԫ��
		diamond.at<uchar>(0, 0) = 0;
		diamond.at<uchar>(0, 1) = 0;
		diamond.at<uchar>(1, 0) = 0;
		diamond.at<uchar>(4, 4) = 0;
		diamond.at<uchar>(3, 4) = 0;
		diamond.at<uchar>(4, 3) = 0;
		diamond.at<uchar>(4, 0) = 0;
		diamond.at<uchar>(4, 1) = 0;
		diamond.at<uchar>(3, 0) = 0;
		diamond.at<uchar>(0, 4) = 0;
		diamond.at<uchar>(0, 3) = 0;
		diamond.at<uchar>(1, 4) = 0;

		//X��Ԫ��
		for (int i = 0; i < 5; ++i)
		{
			x.at<uchar>(i, i) = 1;
			x.at<uchar>(4 - i, i) = 1;
		}
	}
	void setThreshold(int th)
	{
		threshold = th;
	}

	//��Ե���
	cv::Mat getEdges(const cv::Mat &image)
	{
		cv::Mat result;
		cv::morphologyEx(image, result, cv::MORPH_GRADIENT, cv::Mat());
		applyThreshold(result);
		return result;
	}

	//������ֵ��ö�ֵͼ��
	void applyThreshold(cv::Mat& result)
	{
		if (threshold > 0)
		{
			cv::threshold(result, result, threshold, 255, cv::THRESH_BINARY);
		}
	}

	//�ǵ���
	cv::Mat getCornors(const cv::Mat &image)
	{
		cv::Mat result;
		cv::dilate(image, result, cross);		//ʮ��������
		cv::erode(result, result, diamond);		//���θ�ʴ
		cv::Mat result2;
		cv::dilate(image, result2, x);			//X������
		cv::erode(result2, result2, square);	//���θ�ʴ
		
		//ͨ��������ͼ������ֵ�õ��ǵ�ͼ��
		cv::absdiff(result2,result,result);
		applyThreshold(result);
		return result;
	}
	//�ڽǵ㴦��һ��СԲȦ
	void drowOnImage(const cv::Mat& binary, cv::Mat& image)
	{
		cv::Mat_<uchar>::const_iterator it = binary.begin<uchar>();
		cv::Mat_<uchar>::const_iterator itend = binary.end<uchar>();

		//����ÿ������
		for (int i = 0; it != itend; ++it,++i)
		{
			if (*it)
			{
				cv::circle(image, cv::Point(i%image.step, i / image.step), 5, cv::Scalar(255,0,0));
			}
		}
	}

};

int main()
{
	cv::Mat image = cv::imread("test5.jpg",0);
	cv::Mat originimage = cv::imread("test5.jpg", 0);
	cv::namedWindow("image");
	cv::threshold(image,image,180,255,cv::THRESH_BINARY);
	cv::imshow("image", image);

	//��Ե���
	MorphoFeatures morpho;
	morpho.setThreshold(40);
	cv::Mat edges;
	edges = morpho.getEdges(image);
	cv::imshow("edges",edges);

	//�ǵ���
	cv::Mat cornors;
	cornors = morpho.getCornors(image);
	cv::imshow("cornors", cornors);
	morpho.drowOnImage(cornors, originimage);
	cv::imshow("Corners on image", originimage);

	cv::waitKey(0);
	return 0;

}

