#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

/*To binarise the image*/
void binarise(Mat * source, Mat * dst)
{
	Mat * final1 = new Mat();
	int ADAPTIVE_WINDOW_SIZE = 9; // ensure odd number
	int BLUR_WINDOW_SIZE = 5;
	GaussianBlur(*source, *final1, Size_<int>(BLUR_WINDOW_SIZE,BLUR_WINDOW_SIZE), 0, 0, BORDER_DEFAULT );
	adaptiveThreshold(*final1, *dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, ADAPTIVE_WINDOW_SIZE, 4);
	return;
}

int main(int argc, const char** argv)
{

	RNG rng(12345);
	/* Read the image */
	if(argc<1) {cout<<"No image given"<<endl;return -1;}
	string filename = argv[1];
	cout<<filename<<endl;


	#if 1
	Mat img = imread("imgs/"+filename+".jpg", 0);
	Mat maskim = imread("masks/"+filename+".jpg",0);
	// pyrUp( maskim, maskim, Size( img.cols, img.rows ) );
	if (img.empty() || maskim.empty()) {cout << "Error : Image cannot be loaded..!!" << endl;return -1;}

	/* Process the image */
		Mat final1, final;
		binarise(&img,&final1);

		// Mat mask = cv::Mat::zeros(srcImage.size(), srcImage.type());

		Mat dstImage = cv::Mat::zeros(final1.size(), final1.type());    
		final1.copyTo(dstImage, maskim);

		Size_<int> mysz(dstImage.cols+dstImage.cols,dstImage.rows);
		Mat out1 = cv::Mat::zeros(mysz, final1.type()); ;
		int PAD1=15;
		dstImage(cv::Rect(0,0, dstImage.cols,dstImage.rows)).copyTo(out1(cv::Rect(dstImage.cols,0,dstImage.cols,dstImage.rows)));

		namedWindow("masked", WINDOW_NORMAL);
		imshow("masked", out1);
		waitKey(0);
		imwrite("binarised/"+filename+".png",out1);
		destroyWindow("masked");

		// imwrite("binarised/"+filename,dstImage);
	#else
		Mat img = imread("imgs/"+filename, 0);
		Mat bin;
		binarise(&img,&bin);
		cv::Mat out = cv::Mat::zeros(bin.size(), bin.type());
		out.setTo(255);
		cv::Mat out1 = cv::Mat::zeros(bin.size(), bin.type());
		out1.setTo(255);

		int PAD1=15;
		bin(cv::Rect(PAD1,0, bin.cols-PAD1,bin.rows)).copyTo(out1(cv::Rect(0,0,bin.cols-PAD1,bin.rows)));

		int PAD=80;
		bin(cv::Rect(0,0, out1.cols-PAD,out1.rows)).copyTo(out(cv::Rect(PAD,0,out1.cols-PAD,out1.rows)));

		namedWindow("binarised", WINDOW_NORMAL);
		imshow("binarised", out);
		waitKey(0);
		destroyWindow("binarised");
		imwrite("binarised/"+filename,out);
	#endif
return 0;
}
