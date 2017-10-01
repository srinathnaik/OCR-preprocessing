#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv/cv.h"
#include <opencv2/core.hpp>

using namespace cv;
using namespace std;
int main(int argc, char** argv )
{
  RNG rng(12345);
 int largest_area=0;
 int largest_contour_index=0;
 Rect bounding_rect;

 Mat src = imread(argv[1]); //Load source image
 Mat thr(src.rows,src.cols,CV_8UC1); 
 Mat dst(src.rows,src.cols,CV_8UC1,Scalar::all(0));
 cvtColor(src,thr,CV_BGR2GRAY); //Convert to gray
 threshold(thr, thr,25, 255,THRESH_BINARY); //Threshold the gray
 
    vector<vector<Point> > contours; // Vector for storing contour
    vector<Vec4i> hierarchy;

    findContours( thr, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE ); // Find the contours in the image
  
     for( int i = 0; i< contours.size(); i++ ) // iterate through each contour. 
      {
       double a=contourArea( contours[i],false);  //  Find the area of contour
       if(a>largest_area){
       largest_area=a;
       largest_contour_index=i;                //Store the index of largest contour
       bounding_rect=boundingRect(contours[i]); // Find the bounding rectangle for biggest contour
       }
  
      }

      /// Draw contours
      Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
      for( int i = 0; i< contours.size(); i++ )
         {
           Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
           drawContours( drawing, contours, i, color, 4, 8, hierarchy, 0, Point() );
         }

      /// Show in a window
      namedWindow( "Contours", WINDOW_NORMAL );
      imshow( "Contours", drawing );

 // Scalar color(255, 255, 255);
 // cout<<"largest index "<<largest_contour_index<<endl;
 // drawContours( dst, contours,largest_contour_index, color, 4, 8, hierarchy,1); // Draw the largest contour using previously stored index.
 // rectangle(src, bounding_rect,  Scalar(255,0,0),1, 8,0);  
 // namedWindow( "largest Contour", WINDOW_NORMAL );
 // namedWindow( "src", WINDOW_NORMAL );
 // namedWindow( "lines", WINDOW_NORMAL );
 // imshow( "src", src );
 // imshow( "largest Contour", dst );



 //  vector<Vec4i> lines;
 //  Mat cdst = Mat::zeros(dst.rows, dst.cols, CV_8UC3);
 //  // HoughLinesP(dst, lines, 1, CV_PI/180, 50, 200, 10 );
 //  cv::Size size = dst.size();
 //  HoughLinesP(dst, lines, 1, CV_PI/180, 50, size.width / 2.f, 20);
 //  for( size_t i = 0; i < lines.size(); i++ )
 //  {
 //    Vec4i l = lines[i];
 //    line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, 8);

 //  }
 //  imshow( "lines", cdst );

 waitKey(0);
}