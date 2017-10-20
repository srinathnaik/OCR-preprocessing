#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{

	RNG rng(12345);
	/* Read the image */
	if(argc<1) {cout<<"No image given"<<endl;return -1;}
	string filename = argv[1];
	cout<<filename<<endl;
	
	Mat img = imread(filename, 0);
	if (img.empty()) {cout << "Error : Image cannot be loaded..!!" << endl;return -1;}

	/* Process the image */
		Mat final1, final;


		namedWindow("InitialImage", WINDOW_NORMAL);
		// namedWindow("GaussianBlur", WINDOW_NORMAL);
		namedWindow("FinalImage", WINDOW_NORMAL);
		/* binarisation */
		// GaussianBlur(img, final1, Size_<int>(1,1), 0, 0, BORDER_DEFAULT );
		// imshow("GaussianBlur", final1);
		adaptiveThreshold(img, final, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 0);

		imshow("InitialImage", img);
		imshow("FinalImage", final);
		waitKey(0); //wait infinite time for a keypress

		/* splitting the image into two parts */
			/* use hough line transform to detect the lines */
		// Mat canny_output, cdst;
		// Canny(img, canny_output, 50, 200, 3);
		// namedWindow("canny", WINDOW_NORMAL);
		// imshow("canny", canny_output);
 	// 	cvtColor(canny_output, cdst, CV_GRAY2BGR);
		// #if 0
		 //  vector<Vec2f> lines;
		 //  HoughLines(dst, lines, 1, CV_PI/180, 500, 0, 0 );

		 //  for( size_t i = 0; i < lines.size(); i++ )
		 //  {
		 //     float rho = lines[i][0], theta = lines[i][1];
		 //     Point pt1, pt2;
		 //     double a = cos(theta), b = sin(theta);
		 //     double x0 = a*rho, y0 = b*rho;
		 //     pt1.x = cvRound(x0 + 1000*(-b));
		 //     pt1.y = cvRound(y0 + 1000*(a));
		 //     pt2.x = cvRound(x0 - 1000*(-b));
		 //     pt2.y = cvRound(y0 - 1000*(a));
		 //     line( cdst, pt1, pt2, Scalar(0,0,255), 3, CV_AA);
		 //  }
		 // #else
		  // vector<Vec4i> lines;
		  // HoughLinesP(dst, lines, 1, CV_PI/180, 50, 50, 10 );
		  // for( size_t i = 0; i < lines.size(); i++ )
		  // {
		  //   Vec4i l = lines[i];
		//   //   line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, CV_AA);
		//   // }
		//  #endif

		// /* contours */
		// vector<vector<Point> > contours;
  // 		vector<Vec4i> hierarchy;

  // 		/// Find contours
		//   findContours( canny_output, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

		//   /// Draw contours
		//   Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
		//   for( int i = 0; i< contours.size(); i++ )
		//      {
		//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
		//        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
		//      }

		  /// Show in a window
		  // namedWindow( "Contours", WINDOW_NORMAL );
		  // imshow( "Contours", drawing );



		 // namedWindow("source", WINDOW_NORMAL);
		 // imshow("source", img);
		 // namedWindow("detected lines", WINDOW_NORMAL);
		 // imshow("detected lines", cdst);

		 // waitKey();


	/* Display result */
	destroyWindow("InitialImage");
	destroyWindow("GaussianBlur");
	destroyWindow("FinalImage");


return 0;
}
