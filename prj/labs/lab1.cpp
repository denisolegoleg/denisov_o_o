
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

	Mat dst,binRed, binBlue,res;
	cvtColor(image, dst, CV_BGR2HSV);

	inRange(dst, Scalar(0, 255, 255), Scalar(33, 255, 255), binRed);
	inRange(dst, Scalar(120, 180, 140), Scalar(150, 255, 255), binBlue);
	
	Mat temp;
	binRed.copyTo(temp);
	temp += binBlue;

	vector<vector<Point> > contours;
	Mat cont = Mat::zeros(temp.size(), CV_8UC1);

	findContours(temp, contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);
	drawContours(cont, contours, -1, 255, 2, 8);


	return cont;
}

int main(int argc, char* argv[])
{
        // имя картинки задаётся первым параметром
        char* filename = argc == 2 ? argv[1] : "act (9).png";
		char* test_filename = argc == 2 ? argv[1] : "test9.png";
        // получаем картинку
		Mat input_image = imread(filename);
		Mat test = imread(test_filename);

        printf("[i] image: %s\n", filename);
        assert( !input_image.empty() );

		static const char* window_name = "SEEDS Superpixels";
		cv::namedWindow(window_name, 0);
		int num_iterations = 4;
		int prior = 2;
		bool double_step = false;
		int num_superpixels = 400;
		int num_levels = 4;
		int num_histogram_bins = 5;

		Mat result, mask;
		Ptr<SuperpixelSEEDS> seeds;
		int width, height;
		int display_mode = 0;

		width = input_image.size().width;
		height = input_image.size().height;
		
		seeds= createSuperpixelSEEDS(width, height, input_image.channels(), num_superpixels,
			num_levels, prior, num_histogram_bins, double_step);
		
		Mat converted;
		cvtColor(input_image, converted, COLOR_BGR2HSV);

		seeds->iterate(converted, num_iterations);
		result = input_image;

		Mat labels;
		seeds->getLabels(labels);
		//получаем маску
		seeds->getLabelContourMask(mask, true);
		//накладываем маску
		Mat cont = getContour(test);

		int prev = 0;
		int post = 0;
		result.setTo(Scalar(0, 0, 255), cont);
		for (int i = 0; i<result.size().height; i++)
			for (int j = 0; j<result.size().width; j++)
			{
				if (result.at<Vec3b>(i, j) == Vec3b(0, 0, 255))
					prev++;
			}
		result.setTo(Scalar(255, 0, 255), mask);
		
		for (int i = 0; i<result.size().height; i++)
			for (int j = 0; j<result.size().width; j++)
			{
				if (result.at<Vec3b>(i, j) == Vec3b(0, 0, 255))
					post++;
			}

		cout <<((double)post)/(prev)*100 << "% uncrossed" << endl;

		cv::imshow(window_name, result);
		//imshow(window_name, mask);
		cv::waitKey();
        // удаляем окна
        cvDestroyAllWindows();
        return 0;
}