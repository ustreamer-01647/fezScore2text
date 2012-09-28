/*
�L�����N�^�̃X�R�A���C�摜����Ƀe�L�X�g�y�ѐ��l�Ƃ��ēǂݎ��
*/

#include <iostream>
#include <sstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>

#include "Score.h"
#include "recognize.h"
#include "extract.h"
#include "CharacterInfo.h"

static void showrite(const std::string s, cv::Mat &image)
{
	cv::imshow( s, image );
	cv::imwrite( s, image );
}

// �����F��
int recognizeDigit ( const cv::Mat image )
{
	//static int n = 0;
	//std::stringstream ss;
	//ss << "char" << n << ".png";
	//cv::Mat theclone = image.clone();
	//showrite( ss.str(), theclone);
	//n++;

	int nonzero = 0;

	nonzero = cv::countNonZero( image.colRange( image.cols-2, image.cols ));
	if ( image.rows * 2 == nonzero )
	{
		// 1 �E�[2�񂪂��ׂċP�_
		return 1;
	}

	nonzero = cv::countNonZero( image.rowRange( image.rows-2, image.rows ));
	if ( image.cols * 2 == nonzero )
	{
		// 2 ���[2�i�����ׂċP�_
		return 2;
	}

	nonzero = cv::countNonZero( image.rowRange( 0, 2 ));
	if ( image.cols * 2 - 2 < nonzero )
	{
		// 7 ��[2�i�����ׂċP�_�D������2�s�N�Z���܂Ō����Ă��ǂ�
		return 7;
	}

	nonzero = cv::countNonZero( image.colRange( image.cols-3, image.cols-1 ));
	if ( image.rows * 2 == nonzero )
	{
		// 4 �E�[�̍�2�񂪂��ׂċP�_
		return 4;
	}

	nonzero = cv::countNonZero( image( cv::Rect( 0, 0, 1, image.rows*2/3 ) ));
	if ( 0 == nonzero )
	{
		// 3 ���[�̏㕔3����2�����ׂĈÓ_
		return 3;
	}

	nonzero = cv::countNonZero( image( cv::Rect( 0, image.rows/2, 3, 2 ) ));
	if ( 0 == nonzero )
	{
		// ���[�̉������J�n������3x2�����ׂĈÓ_
		return 5;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols/2, image.rows/2-1, 1, 3 ) ));
	if ( 0 == nonzero )
	{
		// �����񒆉�3�s�N�Z�������ׂĈÓ_
		return 0;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols-1, 0, 1, image.rows*2/5 ) ));
	if ( 0 == nonzero )
	{
		// �E�[�㕔5����2�����ׂĈÓ_
		return 6;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols-1, image.rows-3, 1, 3 ) ));
	if ( 0 == nonzero )
	{
		// �E�[����3�s�N�Z�������ׂĈÓ_
		return 9;
	}

	// ��L�����ɓ��Ă͂܂�Ȃ��ꍇ
	return 8;
}

// �������F��
enum Nationality recognizeNationality ( const cv::Mat image )
{
	// yellow hor 40
	// green ces 80
	// blue iel 160
	// purple geb 200
	// red net 



	return Nationality::NUnknown;
}

// �摜���琔�l��ǂݏo��
int recognizeInteger ( const cv::Mat image )
{
	// ��������
	CharactersInfo charactersInfo( image );

	// ���l�F��
	std::stringstream ss;
	for ( size_t n = 0; n < charactersInfo.size(); n++ )
	{
		ss << charactersInfo.getDigit ( n );
	}

	int integer;
	ss >> integer;
	
	return integer;
}

// �L�����N�^�X�R�A�F��
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores )
{
	for ( size_t i = 0; i < images.size(); i++ )
	{
		struct Score score;
		int integer = 0;
		// ����
		integer = recognizeInteger( images[i]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, images[i].rows )));
		score.rank = integer;
		// �L�����N�^��
		//recognizeText ( images[i], Score::NameOffset, Score::NameWidth );
		// ������
		//recognizeNationalityIcon ( images[i], Score::NationalityOffset, Score::NationalityWidth );
		// �N���X
		//recognizeText ( images[i], Score::JobOffset, Score::JobWidth );
		// �L����
		integer = recognizeInteger ( images[i]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, images[i].rows )));
		score.kill = integer;
		// �f�b�h��
		integer = recognizeInteger ( images[i]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, images[i].rows )));
		score.dead = integer;
		// �v���x
		integer = recognizeInteger ( images[i]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, images[i].rows )));
		score.contribution = integer;
		// PC�^�_���[�W
		integer = recognizeInteger ( images[i]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, images[i].rows )));
		score.pcDamage = integer;
		// �����^�_���[�W
		integer = recognizeInteger ( images[i].colRange( Score::ObjectDamageOffset, images[i].cols));
		score.objectDamage = integer;

		score.dump();
		scores.push_back ( score );
	}

}

void recognize ( const cv::Mat scoreTable, const cv::Mat scoreTableBinary, std::vector<struct Score> &scores )
{
	// �J���[�摜
	cv::vector<cv::Mat> scoreRows;
	extractScoreRows( scoreTable, scoreRows );
	
	// 2�l�摜
	cv::vector<cv::Mat> scoreRowsBinary;
	extractScoreRows( scoreTableBinary, scoreRowsBinary );

	for ( size_t n = 0; n < scoreRows.size(); n++ )
	{
		struct Score score;
		int integer = 0;
		enum Nationality nationality = Nationality::NUnknown;
		enum Job job = Job::JUnknown;

		// ����
		integer = recognizeInteger( scoreRowsBinary[n]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, scoreRowsBinary[n].rows )));
		score.rank = integer;
		// �L�����N�^��
		//recognizeText ( scoreRowsBinary[n], Score::NameOffset, Score::NameWidth );
		// ������
		//nationality = recognizeNationalityIcon ( scoreRows[n], Score::NationalityOffset, Score::NationalityWidth );
		//score.nationality = nationality;
		// �N���X
		//recognizeText ( scoreRowsBinary[n], Score::JobOffset, Score::JobWidth );
		// �L����
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, scoreRowsBinary[n].rows )));
		score.kill = integer;
		// �f�b�h��
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, scoreRowsBinary[n].rows )));
		score.dead = integer;
		// �v���x
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, scoreRowsBinary[n].rows )));
		score.contribution = integer;
		// PC�^�_���[�W
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, scoreRowsBinary[n].rows )));
		score.pcDamage = integer;
		// �����^�_���[�W
		integer = recognizeInteger ( scoreRowsBinary[n].colRange( Score::ObjectDamageOffset, scoreRowsBinary[n].cols));
		score.objectDamage = integer;

		score.dump();
		scores.push_back ( score );
	}
}

static int testmain(int argc, char *argv[])
{
	// �e�X�g�摜�I��
	std::vector<std::string> testFiles;
	testFiles.push_back( "300scoreRow.png" );
	testFiles.push_back( "301scoreRow.png" );
	testFiles.push_back( "302scoreRow.png" );
	testFiles.push_back( "303scoreRow.png" );
	testFiles.push_back( "304scoreRow.png" );
	testFiles.push_back( "305scoreRow.png" );
	testFiles.push_back( "306scoreRow.png" );
	testFiles.push_back( "307scoreRow.png" );
	testFiles.push_back( "308scoreRow.png" );
	testFiles.push_back( "309scoreRow.png" );
	// �摜�ǂݍ���
	cv::vector<cv::Mat> testImages;
	for ( size_t i = 0; i <testFiles.size(); i++ )
	{
		cv::Mat image = cv::imread( testFiles[i], 0 );
		testImages.push_back  ( image );
	}

	// �X�R�A�F��
	std::vector<struct Score> scores;
	recognize( testImages, scores );


	return 0;
}

