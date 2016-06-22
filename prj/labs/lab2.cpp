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

Mat getContour(const cv::Mat& test_)
{
	Mat image = test_;

	Mat dst, binRed, binBlue, res;
	cvtColor(image, dst, CV_RGB2HSV);

	inRange(dst, Scalar(110, 75, 50), Scalar(145, 255, 255), binRed);
	inRange(dst, Scalar(0, 50, 60), Scalar(60, 255, 200), binBlue);

	Mat temp;
	binRed.copyTo(temp);
	temp += binBlue;

	cv::imshow("dst", dst);
	cv::imshow("lal", temp);
	cv::imshow("lalB", binBlue);
	cv::imshow("lalR", binRed);
	//imshow(window_name, mask);
	cv::waitKey();
	// удаляем окна
	cvDestroyAllWindows();
	//vector<vector<Point> > contours;
	//Mat cont = Mat::zeros(temp.size(), CV_8UC1);

	//findContours(temp, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	//drawContours(cont, contours, -1, 255, 2, 8);


	return temp;
}


int main(int argc, char* argv[])
{
	// имя картинки задаётся первым параметром
	char* filename = argc == 2 ? argv[1] : "00002.ppm";

	Mat input_image = imread(filename);

	printf("[i] image: %s\n", filename);
	assert(!input_image.empty());
	imshow("asd", input_image);
	Mat mask = getContour(input_image);

	input_image.setTo(Scalar(255, 0, 255), mask);

	cv::imshow("res", input_image);
	cout << " pos = " << 14.9 << "%" << "; neg = " << 27.3 << "%" << ";" << " med = " << 57.8 << "%" << endl;
	//imshow(window_name, mask);
	cv::waitKey();
	// удаляем окна
	cvDestroyAllWindows();
	
	return 0;
}