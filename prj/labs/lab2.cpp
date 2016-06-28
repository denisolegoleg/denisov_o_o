#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/ximgproc.hpp>

#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>

using namespace cv;
using namespace cv::ximgproc;
using namespace std;

Mat getBitMap(const cv::Mat& test_)
{
	Mat image = test_;

	Mat dst, binRed, binBlue, res;
	cvtColor(image, dst, CV_RGB2HSV);

	inRange(dst, Scalar(110, 75, 50), Scalar(145, 255, 255), binRed);
	inRange(dst, Scalar(0, 50, 60), Scalar(60, 255, 200), binBlue);

	Mat temp;
	binRed.copyTo(temp);
	temp += binBlue;

	//Было нужно на этапе проссмотра
	//cv::imshow("dst", dst);
	cv::imshow("lal", temp);
	//cv::imshow("lalB", binBlue);
	//cv::imshow("lalR", binRed);
	
	cv::waitKey();
	// удаляем окна

	cvDestroyAllWindows();



	return temp;
}

double rigthCompare(Mat& larg_, Mat& rarg_)
{
	int temp = 0;
	for (int i = 0; i < larg_.size().height; i++)
		for (int j = 0; j < larg_.size().width; j++)
		{
			if(larg_.at<unsigned char>(i,j) == rarg_.at<unsigned char>(i,j))
			{
				temp++;
			}
		}
	return (double)temp * 100 / (larg_.size().width*larg_.size().height);
}

double errCompare(Mat& larg_, Mat& rarg_)
{
	int temp = 0;
	for (int i = 0; i < larg_.size().height; i++)
		for (int j = 0; j < larg_.size().width; j++)
		{
			if (larg_.at<unsigned char>(i, j) != rarg_.at<unsigned char>(i, j))
			{
				temp++;
			}
		}
	return (double)temp * 100 / (rarg_.size().width*rarg_.size().height);
}

double misCompare(Mat& larg_, Mat& rarg_)
{
	int temp = 0;
	for (int i = 0; i < larg_.size().height; i++)
		for (int j = 0; j < larg_.size().width; j++)
		{
			if ((larg_.at<unsigned char>(i,j) > 0) && (larg_.at<unsigned char>(i, j) != rarg_.at<unsigned char>(i, j)))
			{
				temp++;
			}
		}
	return (double)temp * 100 / (rarg_.size().width*rarg_.size().height);
}

int main(int argc, char* argv[])
{
	// имя картинки задаётся первым параметром
	char* filename = argc == 2 ? argv[1] : "00002.ppm";
	char* testname = "00002.jpg";
	Mat test_img = imread(testname);
	Mat input_image = imread(filename);

	printf("[i] image: %s\n", filename);
	assert(!input_image.empty());
	imshow("asd", input_image);
	Mat mask = getBitMap(input_image);
	Mat test_mask = getBitMap(test_img);

	double right = rigthCompare(mask, test_mask);
	double err = errCompare(mask, test_mask);
	double mis = misCompare(mask, test_mask);
	
	cout << "After comparassion " << endl << "Right detection = " << right << "%" << endl << "Wrong detection = " << err << "%" << endl << "Misdetection = " << mis << "%" << endl;

	//Наложение 
	input_image.setTo(Scalar(255, 0, 255), mask);

	cv::imshow("res", input_image);
	//imshow(window_name, mask);
	cv::waitKey();
	// удаляем окна
	cvDestroyAllWindows();
	
	return 0;
}