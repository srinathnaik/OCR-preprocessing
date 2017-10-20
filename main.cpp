#include <iostream>
#include "driver.cpp"
using namespace cv;
using namespace std;

void cut(int argc, const char** argv)
{

	RNG rng(12345);
	string filename;bool display;
	display=0;filename="";
	if(argc>1){if(argv[2]=="1"){display=1;}; argc--;}
	if(argc>0){filename=argv[1];argc--;}

	/*read the image */
		if(filename=="") {cout<<"No image given"<<endl;return ;}
		cout<<filename<<endl;
		Mat img = imread(filename, 0);
		if (img.empty()) {cout << "Error : Image cannot be loaded..!!" << endl;return ;}
		filename = filename.substr(0,filename.length()-4);

	/*binarise*/
		Mat * s1 = new Mat();
		binarise(&img,s1);
		if(display)show(s1,"I1 : binarised");

	/*transform*/
		Mat * s2 = new Mat();
		morphological_transform(s1,s2,4);
		if(display)show(s2,"I2 : transformed");

	/*countour bound*/
		Mat s3 = draw_contours(s2);
		if(display)show(&s3,"I3 : contours");

	/*adjust rotation of image*/
		double ang = rotate_angle(&s3);
		// get rotation matrix for rotating the image around its center
		cv::Point2f center(s3.cols/2.0, s3.rows/2.0);
		cv::Mat rot = cv::getRotationMatrix2D(center, ang, 1.0);
		// determine bounding rectangle
		cv::Rect bbox = cv::RotatedRect(center,s3.size(), ang).boundingRect();
		// adjust transformation matrix
		rot.at<double>(0,2) += bbox.width/2.0 - center.x;
		rot.at<double>(1,2) += bbox.height/2.0 - center.y;

		cv::Mat img1;
		cv::warpAffine(*s2, img1, rot, bbox.size(),INTER_LINEAR,BORDER_CONSTANT,colorTab[5]);
		cv::warpAffine(img, img, rot, bbox.size(),INTER_LINEAR,BORDER_CONSTANT,colorTab[6]); // original image
		// cv::warpAffine(s3, img1, rot, bbox.size(),INTER_LINEAR,BORDER_CONSTANT,Scalar(0, 0, 0));
		if(display)show(&img1,"rotated image");

	if(display)waitKey(0);
	if(display)destroyWindow("I1 : binarised");
	if(display)destroyWindow("I2 : transformed");
	if(display)destroyWindow("I3 : contours");
	if(display)destroyWindow("rotated image");
	// show(&img1,"rotated image");

	/*draw contours for the rotated image*/
		s3 = draw_contours(&img1);
		if(display)show(&s3,"II1 : contours");

		// draw_contours1(&img1);

	/*detect mid-line*/
		LINE midline = detect_midline(&s3);

	/*adjust rotation of image again according to midline*/
		Mat img2;
		ang = (90.0+midline.slope);
		// get rotation matrix for rotating the image around its center
		center.x=midline.pt.x; center.y=midline.pt.y;
		rot = cv::getRotationMatrix2D(center, ang, 1.0);
		// determine bounding rectangle
		bbox = cv::RotatedRect(center,img1.size(), ang).boundingRect();

		cv::warpAffine(img1, img2, rot, bbox.size(),INTER_LINEAR,BORDER_CONSTANT,colorTab[5]);
		cv::warpAffine(img, img, rot, bbox.size(),INTER_LINEAR,BORDER_CONSTANT,colorTab[6]);
		
		/*draw mid line now*/
		Point pt1, pt2;float theta =  90 * CV_PI/180;
		double a = cos(theta), b = sin(theta);
		double x0 = midline.pt.x , y0 = midline.pt.y;
		pt1.x = x0- 5000*(a);
		pt1.y = y0- 5000*(b);
		pt2.x = cvRound(x0 + 5000*(a));
		pt2.y = cvRound(y0 + 5000*(b));
		line( img2, pt1, pt2, colorTab[0], 3, 8);

		show(&img2,"final rotated image");

	/*crop the image*/
		int PADDING = 8;
		Mat left,leftbm,right,rightbm;
		int WIDTH = img2.cols-(x0+PADDING);
		int HEIGHT = img2.rows;
		cout<<" right crop part "<<x0+PADDING<<" 0 "<<WIDTH<<" "<<HEIGHT<<endl;
		right = img(Rect(x0+PADDING,0,WIDTH,HEIGHT));
		rightbm = img2(Rect(x0+PADDING,0,WIDTH,HEIGHT));
		show(&right,"right part");

		imwrite(filename+"_r.png",right);

		WIDTH = x0-PADDING;
		cout<<" left crop part 0 0 "<<WIDTH<<" "<<HEIGHT<<endl;
		left = img(Rect(0,0,WIDTH,HEIGHT));
		leftbm = img2(Rect(0,0,WIDTH,HEIGHT));
		show(&left,"left part");

		imwrite(filename+"_l.png",left);
		waitKey(0);
		destroyWindow("rotated image");
		if(display)destroyWindow("II1 : contours");
		destroyWindow("final rotated image");
		destroyWindow("right part");

	/*black-model of left and right parts*/
return ;
}

int main(int argc, const char** argv)
{
	cut(argc,argv);
	return 0;
}
