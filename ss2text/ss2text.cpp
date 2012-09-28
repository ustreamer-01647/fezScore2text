#include "Score.h"
#include "extract.h"
#include "recognize.h"

#include <opencv2/highgui/highgui.hpp>

int main ( int argc, char *argv[] )
{
	// ss読み込み
	cv::Mat ss = cv::imread("ss.png");

	// 切り出し
	cv::Mat scoreTable = extractScoreTable( ss );

	// カラー画像と2値画像を作る
	cv::Mat scoreTableBinary = scoreTable2Binary( scoreTable );

	// スコア読み出し
	std::vector<Score> scores;
	recognize( scoreTable, scoreTableBinary, scores );

	// 表示

	return 0;
}
