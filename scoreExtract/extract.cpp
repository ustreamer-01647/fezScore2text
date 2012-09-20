#include "extract.h"

#include <sstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>

#ifdef _DEBUG
    //Debug���[�h�̏ꍇ
    #pragma comment(lib,"opencv_core242d.lib")
    #pragma comment(lib,"opencv_highgui242d.lib")
    #pragma comment(lib,"opencv_imgproc242d.lib")
    #pragma comment(lib,"opencv_features2d242d.lib")
#else
    //Release���[�h�̏ꍇ
    #pragma comment(lib,"opencv_core242.lib")
    #pragma comment(lib,"opencv_highgui242.lib")
    #pragma comment(lib,"opencv_imgproc242.lib")
    #pragma comment(lib,"opencv_features2d242.lib")
#endif

void showrite(const std::string s, cv::Mat &image)
{
	cv::imshow( s, image );
	cv::imwrite( s, image );
}

// extractScoreTable() �Ŏg�p����
const int ExtractOffsetX = 362;
const int ExtractOffsetY = 176;
const int TableWidth = 720;
const int TableHeight = 352;

cv::Mat extractScoreTable ( const cv::Mat ss )
{
	// �摜����������I�t�Z�b�g�ʒu�̈���`
	return ss( cv::Rect( 
		ss.cols / 2 - ExtractOffsetX,
		ss.rows / 2 - ExtractOffsetY,
		TableWidth, TableHeight));
}

// extractRows() �Ŏg�p����
const int ScoreRowHeight = 17;
const int ScoreRowInterval = 15;
const int ScoreTop10Rows = 10;

void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows )
{
	// �����L���O�X�R�A10��
	for ( int i = 0; i < ScoreTop10Rows; i++ )
	{
		cv::Mat scoreRow = 
			scoreTable( cv::Rect( 
			0, i * ( ScoreRowHeight + ScoreRowInterval ), scoreTable.cols, ScoreRowHeight ) );
		scoreRows.push_back ( scoreRow );
		
		std::stringstream ss;
		ss << "30" << i << "scoreRow.png"; 
		showrite( ss.str(), scoreRow );
	}

	// ���L�����N�^�X�R�A
	cv::Mat scoreRow = 
			scoreTable( cv::Rect( 
			0, scoreTable.rows - ScoreRowHeight, scoreTable.cols, ScoreRowHeight ) );
		scoreRows.push_back ( scoreRow );
	showrite( "35scoreRow.png", scoreRow );
}


int main(int argc, char *argv[])
{
	cv::Mat ss = cv::imread("ss.png");

	/*
	 width 720 height 352
	 1280x800 �ł� xoffset 278 yoffset 224 
	 �摜��������
	 ���� 15:38 (paulga) 362 [1280/2-278]
	 ��� 15:42 (paulga) 176 [800/2-224]
	 �ړ������ʒu���n�_�Ƃ���C�O�q�����T�C�Y�̋�`�ɒ��ڂ���
	*/


	// �����L���O�\�����𒊏o����
	cv::Mat scoreTable = extractScoreTable( ss );
	showrite("10scoreTable.png", scoreTable);

	// �e�L�����N�^�X�R�A�s�𒊏o����
	
	// �O���C�X�P�[����
	cv::Mat gray;
	cv::cvtColor( scoreTable, gray, CV_BGR2GRAY );
	showrite( "15grayscale.png", gray );

	// 2�l��
	cv::Mat bin;
	cv::threshold( gray, bin, 150.0, 255.0, CV_THRESH_BINARY );
	showrite( "20binary.png", bin );

	// �s���o
	cv::vector<cv::Mat> scoreRows;
	extractScoreRows( bin, scoreRows );

	cv::waitKey();

	return 0;
}

