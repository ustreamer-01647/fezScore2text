#include "Score.h"
#include "extract.h"
#include "recognize.h"

#include <opencv2/highgui/highgui.hpp>

int main ( int argc, char *argv[] )
{
	// ss�ǂݍ���
	cv::Mat ss = cv::imread("ss2.png");

	// �X�R�A�ǂݏo��
	std::vector<Score> scores;
	recognize( ss, scores );

	// �\��
	cv::waitKey();

	return 0;
}
