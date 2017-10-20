#include <iostream>
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;
RNG rng(12345);
Scalar colorTab[] =
	{
		Scalar(0, 0, 255),	//0->red
		Scalar(0,255,0),	//1->green	
		Scalar(255,0,0),	//2->blue
		Scalar(255,0,255),	//3->
		Scalar(0,255,255),	//4->	
		Scalar(255,255,255),//5->white
		Scalar(0,0,0)		//6-black
	};

class LINE
{
public:
	Point pt;
	double slope;
	LINE(int x,int y,double angle)
	{
		pt.x=x;
		pt.y=y;
		slope=angle;// angle in degrees generally
	}
};

/*To binarise the image*/
void binarise(Mat * source, Mat * dst)
{
	Mat * final1 = new Mat();
	int ADAPTIVE_WINDOW_SIZE = 11; // ensure odd number
	int BLUR_WINDOW_SIZE = 5;
	GaussianBlur(*source, *final1, Size_<int>(BLUR_WINDOW_SIZE,BLUR_WINDOW_SIZE), 0, 0, BORDER_DEFAULT );
	adaptiveThreshold(*final1, *dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, ADAPTIVE_WINDOW_SIZE, 4);
	return;
}

/*To display the image*/
void show(Mat * im, string title)
{
	namedWindow(title, WINDOW_NORMAL);
	imshow(title, *im);
	return;
}

/*Appylyting a morphological transform*/
void morphological_transform(Mat * src, Mat * dst, int morph_size)
{
	int operation =  MORPH_OPEN;
	int morph_elem = 2;// elliptical window
	Mat element = getStructuringElement( morph_elem, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
	morphologyEx( *src, *dst, operation, element );
	return;
}

/*Largest contour detection*/
Mat draw_contours(Mat * src)
{
	Mat dst((*src).rows,(*src).cols,CV_8UC1,Scalar::all(0));
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours( *src, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
	double largest_area = 0.0;
	int largest_contour_index = 0;
	for( int i = 0; i< contours.size(); i++ )
	{
		double a=contourArea( contours[i],false);
		if(a>largest_area)
		{
			largest_area=a;
			largest_contour_index=i;
			// bounding_rect=boundingRect(contours[i]);
		}
	}

	int THICKNESS_OF_LINE=4;
	int CONNECTEDNESS=8;
	drawContours( dst, contours,largest_contour_index, colorTab[5], THICKNESS_OF_LINE, CONNECTEDNESS, hierarchy,1);

	return dst;
}

/*Get rotation angle based on text*/
double rotate_angle(Mat *src)
{
	double angle;
	vector<vector<double> > A(4,vector<double>());

	vector<Vec4i> lines;
	Mat cdst = Mat::zeros((*src).rows, (*src).cols, CV_8UC3);

	int MIN_VOTES = 50;
	int MIN_LENGTH = 200;
	int MIN_GAP = 10;
	HoughLinesP((*src), lines, 1, CV_PI/180, MIN_VOTES, MIN_LENGTH, MIN_GAP );
	
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		angle = atan2((double)l[3] - l[1],
	                   (double)l[2] - l[0]);
		angle = (angle*180)/CV_PI;

		/*put the angle into one of the 4 buckets*/
		if(0<=angle && angle<=45){A[0].push_back(angle);if(angle!=0)line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), colorTab[0], 3, 8);}
		else if(45<angle && angle<90){A[1].push_back(angle);line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), colorTab[1], 3, 8);}
		else if(-90<=angle && angle<-45){A[2].push_back(angle);if(angle!=-90)line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), colorTab[2], 3, 8);}
		else if(angle>=-45 && angle<0){A[3].push_back(angle);line( cdst, Point(l[0], l[1]), Point(l[2], l[3]), colorTab[3], 3, 8);}
		// else cout<<"different : "<<angle<<endl;
	}

	cout<<"total : "<<lines.size()<<endl;
	cout<<"0-45 : "<<A[0].size()<<" || 45-90 : "<<A[1].size()<<" || (-45)-(-90) : "<<A[2].size()<<" || (0)-(-45) : "<<A[3].size()<<endl;
	std::vector<double> means(4,0);
	double val=0.0;int n;
	n=A[0].size();
	for(int i=0;i<n;i++)val+=A[0][i];
	means[0]=val/n;cout<<"mean 0-45 : "<<means[0]<<" "<<endl;

	val=0.0;n=A[1].size();
	for(int i=0;i<n;i++)val+=A[1][i];
	means[1]=val/n;cout<<"mean 45-90 : "<<means[1]<<" "<<endl;

	val=0.0;n=A[2].size();
	for(int i=0;i<n;i++)val+=A[2][i];
	means[2]=val/n;cout<<"mean (-45)-(-90) : "<<means[2]<<" "<<endl;

	val=0.0;n=A[3].size();
	for(int i=0;i<n;i++)val+=A[3][i];
	means[3]=val/n;cout<<"mean (0)-(-45) : "<<means[3]<<" "<<endl;


	int maxi=INT_MIN;int ind=-1;
	for(int i=0;i<4;i++)
	{
		if((int)A[i].size()>maxi){maxi = A[i].size();ind=i;}
	}


	/*transform image so as to remove frame border lines*/
	//Mat frame = *src;
	//cv::Mat out = cv::Mat::zeros(frame.size(), frame.type());
	//frame(cv::Rect(10,10, frame.cols,frame.rows-10)).copyTo(out(cv::Rect(0,0,frame.cols,frame.rows-10)));

	double ang = means[ind];
	cout<<"estimated orientation of book is : "<<ind<<" .. "<<ang<<endl;

	return ang;
}

/*Midline Detection*/
LINE detect_midline(Mat * src)
{
	double angle;
	vector<Vec4i> lines;
	Mat cdst = Mat::zeros((*src).rows, (*src).cols, CV_8UC3);

	int MIN_VOTES = 50;
	int MIN_LENGTH = 200;
	int MIN_GAP = 10;
	HoughLinesP((*src), lines, 1, CV_PI/180, MIN_VOTES, MIN_LENGTH, MIN_GAP);

	/*group lines into horizontal and vertical*/
	vector<Point3_<float> > x_lines,y_lines;int y_avg = 0;
	for( size_t i = 0; i < lines.size(); i++ )
	{
		Vec4i l = lines[i];
		angle = atan2((double)l[3] - l[1],
						(double)l[2] - l[0]);
		angle = (angle*180)/CV_PI;

		Point3_<float> my_point(l[0],l[2],angle);
		if(-5<=angle && angle<=5)
		{
			x_lines.push_back(my_point);line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), colorTab[0], 3, 8);	
		}
		else if((85<=angle && angle<=90)||(-90<=angle && angle<=-85))
		{
			y_lines.push_back(my_point);line(cdst, Point(l[0], l[1]), Point(l[2], l[3]), colorTab[1], 3, 8);
			y_avg+=l[1]+l[3];
		}
	}

	/*get 3 clusters of vertical lines*/
	int clusterCount = 3;
	Mat centers;
	int N = (int)y_lines.size();y_avg = y_avg/(2*N);
	std::vector<int> labels;
	kmeans(y_lines, clusterCount, labels,
			TermCriteria( TermCriteria::EPS+TermCriteria::COUNT, 10, 1.0),
			3, KMEANS_PP_CENTERS, centers);

	Size_<int> ss = centers.size();
	cout<<"size of centers "<<ss.height<<" "<<ss.width<<endl;

	cout<<" cluster 1 : "<<centers.at<float>(0,0)<<" "<<centers.at<float>(0,1)<<" "<<centers.at<float>(0,2)<<endl;
	cout<<" cluster 2 : "<<centers.at<float>(1,0)<<" "<<centers.at<float>(1,1)<<" "<<centers.at<float>(1,2)<<endl;
	cout<<" cluster 3 : "<<centers.at<float>(2,0)<<" "<<centers.at<float>(2,1)<<" "<<centers.at<float>(2,2)<<endl;

	vector<int> avg_x;
	avg_x.push_back((centers.at<float>(0,0)+centers.at<float>(0,1))/2);
	avg_x.push_back((centers.at<float>(1,0)+centers.at<float>(1,1))/2);
	avg_x.push_back((centers.at<float>(2,0)+centers.at<float>(2,1))/2);

	int fx; double slp;
	int first,second;
	first = INT_MIN;second=INT_MIN;
	for(int i=0;i<avg_x.size();i++)
	{
		if(avg_x[i]>first){second=first;first=avg_x[i];}
		else if(avg_x[i]>second){second=avg_x[i];}
	}
	for(int i=0;i<avg_x.size();i++)
	{
		if(avg_x[i]==second){fx=avg_x[i];slp=centers.at<float>(i,2);break;}
	}

	/*y_avg is taken from all vertical lines(may not necessarily belong to this cluster)*/
	cout<<"mid-line is ("<<fx<<","<<y_avg<<") "<<slp<<endl;

	Point pt1, pt2;float theta =  slp * CV_PI/180;
	double a = cos(theta), b = sin(theta);
	double x0 = fx , y0 = y_avg;
	pt1.x = x0- 5000*(a);
	pt1.y = y0- 5000*(b);
	pt2.x = cvRound(x0 + 5000*(a));
	pt2.y = cvRound(y0 + 5000*(b));
	line( cdst, pt1, pt2, colorTab[3], 3, CV_AA);
	show(&cdst,"II2 : mid-line");

	LINE midline(x0,y0,slp);
	return midline;
}