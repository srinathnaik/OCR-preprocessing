
void compute_skew(Mat * source, Mat * dst)
{
	// Load in grayscale.
	Mat src = *source; 
	Size size = src.size();
	bitwise_not(src, src);

	std::vector<cv::Vec4i> lines;
	cv::HoughLinesP(src, lines, 1, CV_PI/180, 100, size.width / 2.f, 20);

	cv::Mat disp_lines(size, CV_8UC1, cv::Scalar(0, 0, 0));
	double angle = 0.;
	unsigned nb_lines = lines.size();
	for (unsigned i = 0; i < nb_lines; ++i)
	{
	    cv::line(disp_lines, cv::Point(lines[i][0], lines[i][1]),
	             cv::Point(lines[i][2], lines[i][3]), cv::Scalar(255, 0 ,0));
	    angle += atan2((double)lines[i][3] - lines[i][1],
	                   (double)lines[i][2] - lines[i][0]);
	}
	angle /= nb_lines; // mean angle, in radians.

	std::cout << "approx text angle "<< angle * 180 / CV_PI << std::endl;

	namedWindow( "rotate : lines", WINDOW_NORMAL );
	cv::imshow("rotate : lines", disp_lines);
	cv::waitKey(0);
	cv::destroyWindow(filename);
}
