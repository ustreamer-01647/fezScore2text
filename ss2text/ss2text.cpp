#include "Score.h"
#include "extract.h"
#include "recognize.h"

#include <opencv2/highgui/highgui.hpp>

int main ( int argc, char *argv[] )
{
	// ss�ǂݍ���
	cv::Mat ss = cv::imread("ss.png");

	// �؂�o��
	cv::Mat scoreTable = extractScoreTable( ss );

	// �J���[�摜��2�l�摜�����
	cv::Mat scoreTableBinary = scoreTable2Binary( scoreTable );

	// �X�R�A�ǂݏo��
	std::vector<Score> scores;
	recognize( scoreTable, scoreTableBinary, scores );

	// �\��

	return 0;
}
