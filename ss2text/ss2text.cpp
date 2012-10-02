#include "Score.h"
#include "extract.h"
#include "recognize.h"

#include <opencv2/highgui/highgui.hpp>

int main ( int argc, char *argv[] )
{
	// ss読み込み
	cv::Mat ss = cv::imread("ss3.png");

	// スコア読み出し
	std::vector<Score> scores;
	recognize( ss, scores );

	for ( size_t n = 0; n < scores.size(); n++ )
	{
		std::cout << scores[n].toString() << std::endl;
	}

	// 表示
	cv::waitKey();

	return 0;
}
