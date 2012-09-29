#include "Score.h"
#include "extract.h"
#include "recognize.h"

#include <opencv2/highgui/highgui.hpp>

int main ( int argc, char *argv[] )
{
	// ss読み込み
	cv::Mat ss = cv::imread("ss2.png");

	// スコア読み出し
	std::vector<Score> scores;
	recognize( ss, scores );

	// 表示
	cv::waitKey();

	return 0;
}
