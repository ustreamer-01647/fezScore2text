#include "Score.h"
#include "extract.h"
#include "recognize.h"

#include <opencv2/highgui/highgui.hpp>

int main ( int argc, char *argv[] )
{
	// ss�ǂݍ���
	cv::Mat ss = cv::imread("ss3.png");

	// �X�R�A�ǂݏo��
	std::vector<Score> scores;
	recognize( ss, scores );

	for ( size_t n = 0; n < scores.size(); n++ )
	{
		std::cout << scores[n].toString() << std::endl;
	}

	// �\��
	cv::waitKey();

	return 0;
}
