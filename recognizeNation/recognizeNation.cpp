#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>

#include <iostream>

int main (int argc, char *argv[])
{
	cv::Mat ces = cv::imread ( "ces.png", 0 );
	cv::Mat geb = cv::imread ( "geb.png", 0 );
	cv::Mat hor = cv::imread ( "hor.png", 0 );

	std::cout << "ces: " << cv::countNonZero(ces) << std::endl;
	std::cout << "geb: " << cv::countNonZero(geb) << std::endl;
	std::cout << "hor: " << cv::countNonZero(hor) << std::endl;

	return 0;
}

//ces: 25
//geb: 52
//hor: 99
