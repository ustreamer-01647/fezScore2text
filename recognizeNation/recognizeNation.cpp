// 国家アイコンの特徴調査用プログラム

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include <iostream>

int main (int argc, char *argv[])
{
	cv::Mat ces = cv::imread ( "ces.png", 0 );
	cv::Mat geb = cv::imread ( "geb.png", 0 );
	cv::Mat hor = cv::imread ( "hor.png", 0 );
	cv::Mat iel130 = cv::imread ( "ielbin130.png", 0 );
	cv::Mat net130 = cv::imread ( "netbin130.png", 0 );

	std::cout << "ces: " << cv::countNonZero(ces) << std::endl;
	std::cout << "geb: " << cv::countNonZero(geb) << std::endl;
	std::cout << "hor: " << cv::countNonZero(hor) << std::endl;
	std::cout << "iel: " << cv::countNonZero(iel130.rowRange(2, iel130.rows)) << std::endl;
	std::cout << "net: " << cv::countNonZero(net130.rowRange(2, net130.rows)) << std::endl;

	cv::Mat iel = cv::imread ( "iel.png" );
	cv::Mat net = cv::imread ( "net.png" );

	cv::Mat ag, bg;
	cv::cvtColor( iel, ag, CV_BGR2GRAY );
	cv::cvtColor( net, bg, CV_BGR2GRAY );

	cv::Mat ab, bb;
	cv::threshold( ag, ab, 130.0, 255.0, CV_THRESH_BINARY );
	cv::threshold( bg, bb, 130.0, 255.0, CV_THRESH_BINARY );

	cv::imshow( "iel", ab );
	cv::imshow( "net", bb );
	cv::imwrite("ielbin130.png", ab);
	cv::imwrite("netbin130.png", bb);

	cv::waitKey();


	/* grubCut無理
	cv::Mat ielMask = cv::Mat::zeros( iel.size(), CV_8UC1 );
	// 両端1列ずつを背景指定
	for ( int row = 0; row < ielMask.rows; row++ )
	{
		ielMask.at<unsigned char>( cv::Point( 0, row ) ) = cv::GC_BGD;
		ielMask.at<unsigned char>( cv::Point( ielMask.cols-1, row ) ) = cv::GC_BGD;
	}
	// 中央4画素を前景指定
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
	*/

	return 0;
}

//ces: 25
//geb: 52
//hor: 99
//iel: 7
//net: 23
