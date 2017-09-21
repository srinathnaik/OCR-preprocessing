#include <iostream>
#include <string>
#include <opencv2/core.hpp>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, const char** argv)
{
	/* Read the image */
	if(argc<1) {cout<<"No image given"<<endl;return -1;}
	string filename = argv[1];
	cout<<filename<<endl;
	
	Mat img = imread(ptr, 0);
	if (img.empty()) {cout << "Error : Image cannot be loaded..!!" << endl;return -1;}

	/* Process the image */
	Mat * final1 = new Mat();
	Mat * final = new Mat();
	GaussianBlur(img, *final1, Size_<int>(5,5), 0, 0, BORDER_DEFAULT );
	adaptiveThreshold(*final1, *final, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 4);

	/* Display result */
	namedWindow("InitialImage", WINDOW_NORMAL);
	imshow("InitialImage", img);
	namedWindow("FinalImage", WINDOW_NORMAL);
	imshow("FinalImage", *final);
	waitKey(0); //wait infinite time for a keypress
	destroyWindow("InitialImage");
	destroyWindow("FinalImage");


return 0;
}
