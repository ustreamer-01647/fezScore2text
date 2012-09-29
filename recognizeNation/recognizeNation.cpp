#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <iostream>

int main (int argc, char *argv[])
{
	cv::Mat ces = cv::imread ( "ces.png", 0 );
	cv::Mat geb = cv::imread ( "geb.png", 0 );
	cv::Mat hor = cv::imread ( "hor.png", 0 );

	std::cout << "ces: " << cv::countNonZero(ces) << std::endl;
	std::cout << "geb: " << cv::countNonZero(geb) << std::endl;
	std::cout << "hor: " << cv::countNonZero(hor) << std::endl;

	cv::Mat iel = cv::imread ( "iel.png" );
	cv::Mat net = cv::imread ( "net.png" );

	cv::Mat ielMask = cv::Mat::zeros( iel.size(), CV_8UC1 );
	// —¼’[1—ñ‚¸‚Â‚ð”wŒiŽw’è
	for ( int row = 0; row < ielMask.rows; row++ )
	{
		ielMask.at<unsigned char>( cv::Point( 0, row ) ) = cv::GC_BGD;
		ielMask.at<unsigned char>( cv::Point( ielMask.cols-1, row ) ) = cv::GC_BGD;
	}
	// ’†‰›4‰æ‘f‚ð‘OŒiŽw’è
	ielMask.at<unsigned char>( cv::Point( ielMask.cols/2,   ielMask.rows/2 ) ) = cv::GC_FGD;
	ielMask.at<unsigned char>( cv::Point( ielMask.cols/2+1, ielMask.rows/2 ) ) = cv::GC_FGD;
	ielMask.at<unsigned char>( cv::Point( ielMask.cols/2+1, ielMask.rows/2+1 ) ) = cv::GC_FGD;
	ielMask.at<unsigned char>( cv::Point( ielMask.cols/2,   ielMask.rows/2+1 ) ) = cv::GC_FGD;

	cv::Mat bgdModel;
	cv::Mat fgdModel;
	cv::Rect rect = cv::Rect( 0, 0, iel.cols, iel.rows );
	cv::grabCut( iel, ielMask, rect, bgdModel, fgdModel, 5, cv::GC_INIT_WITH_MASK );

	cv::imshow ( "bg", bgdModel );
	cv::imshow ( "fg", fgdModel );

	cv::waitKey();

	return 0;
}

//ces: 25
//geb: 52
//hor: 99
