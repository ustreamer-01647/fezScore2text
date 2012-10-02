/*
�L�����N�^�̃X�R�A���C�摜����Ƀe�L�X�g�y�ѐ��l�Ƃ��ēǂݎ��
*/

#include <iostream>
#include <sstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

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
		// 7 ��[2�i�����ׂċP�_�D������1�s�N�Z���܂Ō����Ă��ǂ�
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
	// 2�l�摜�̋P�_������ɔ��ʂ���
	// ces: 25
	// geb: 52
	// hor: 99
	// http://twitpic.com/az9oy2

	int count = cv::countNonZero( image );
	if ( count > 80 )
		return Nationality::NHordaine;

	if ( count > 45 )
		return Nationality::NGeburand;

	if ( count > 15 )
		return Nationality::NCesedria;

	return Nationality::NUnknown;
}

// �������F��
// recognizeNationality�Ŕ��f�ł��Ȃ������ꍇ�C�J���[�摜����ɔ��ʂ���
enum Nationality recognizeNationality2 ( const cv::Mat image )
{
	// 臒l130.0��2�l�������摜����ɂ���

	cv::Mat gray;
	cv::cvtColor( image, gray, CV_BGR2GRAY );
	cv::Mat bin;
	cv::threshold( gray, bin, 130.0, 255.0, CV_THRESH_BINARY );
	
	// iel: 7
	// net: 23
	// http://twitpic.com/azbzjh

	int count = cv::countNonZero( bin.rowRange( 2, bin.rows ) );
	if ( count > 15 )
		return Nationality::NNetzawar;

	if ( count > 3 )
		return Nationality::NIelsord;

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

// �N���X�F��
enum Job recognizeJob ( const cv::Mat image )
{

	// ��������
	// �u���v��2��������
	CharactersInfo charactersInfo ( image );

	// �����m�F�o��
	static int counter = 0;
	std::stringstream ss;
	for ( size_t n = 0; n < charactersInfo.size(); n++ )
	{
		ss << "char" << counter << "x" << n << ".png";
		showrite( ss.str(), charactersInfo.characterImage(n));
		ss.str("");
	}
	counter++;

	// 6���F�E�H���A�[
	if ( 6 == charactersInfo.size() )
	{
		return Job::JWarrior;
	}

	// 5���F�\�[�T���[�C�t�F���T�[
	if ( 5 == charactersInfo.size() )
	{
		int second = cv::countNonZero( charactersInfo.characterImage( 1 ) );
		int fifth = cv::countNonZero( charactersInfo.characterImage( 4 ) );
		// ��2���Ƒ�5���̋P�_������3�ȉ��Ȃ瓯�ꕶ���Ƃ݂Ȃ��C�\�[�T���[����
		int diff = second - fifth;
		if ( diff * diff < 3 * 3 )
		{
			return Job::JSorcerer;
		}
		return Job::JFencer;
	}

	// 4���F�X�J�E�g�C�Z�X�^�X
	if ( 4 == charactersInfo.size() )
	{
		int second = cv::countNonZero( charactersInfo.characterImage( 1 ) );
		int fourth = cv::countNonZero( charactersInfo.characterImage( 3 ) );
		// ��2���Ƒ�4���̋P�_������3�ȉ��Ȃ瓯�ꕶ���Ƃ݂Ȃ��C�Z�X�^�X����
		int diff = second - fourth;
		if ( diff * diff < 3 * 3 )
		{
			return Job::JCestas;
		}
		return Job::JScout;
	}

	return Job::JUnknown;
}

// �L�����N�^�X�R�A�F��
// �g��Ȃ�
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

/** �L�����N�^�X�R�A�F��
@param ss fez�̃X�N���[���V���b�g
@param scores �F�������X�R�A�f�[�^���͐�
*/
void recognize ( const cv::Mat ss, std::vector<struct Score> &scores )
{
	// �؂�o��
	cv::Mat scoreTable = extractScoreTable( ss );

	// 2�l�摜�����
	cv::Mat scoreTableBinary = scoreTable2Binary( scoreTable );
	//showrite( "table.png", scoreTableBinary );

	// �L�����N�^�X�R�A�؂�o��
	cv::vector<cv::Mat> scoreRows;
	extractScoreRows( scoreTableBinary, scoreRows );

	for ( size_t n = 0; n < scoreRows.size(); n++ )
	{
		struct Score score;
		int integer = 0;
		enum Nationality nationality = Nationality::NUnknown;
		enum Job job = Job::JUnknown;

		// ����
		integer = recognizeInteger( scoreRows[n]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, scoreRows[n].rows )));
		score.rank = integer;
		// �L�����N�^��
		//recognizeText ( scoreRows[n], Score::NameOffset, Score::NameWidth );
		// ������
		nationality = recognizeNationality ( scoreRows[n] ( cv::Rect( Score::NationalityOffset, 0, Score::NationalityWidth, scoreRows[n].rows )));
		if ( nationality == NUnknown )
		{
			nationality = recognizeNationality2( extractColorNationality( scoreTable, n) );
		}
		score.nationality = nationality;
		// �N���X
		job = recognizeJob ( scoreRows[n]( cv::Rect (Score::JobOffset, 0, Score::JobWidth, scoreRows[n].rows )));
		score.job = job;
		// �L����
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, scoreRows[n].rows )));
		score.kill = integer;
		// �f�b�h��
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, scoreRows[n].rows )));
		score.dead = integer;
		// �v���x
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, scoreRows[n].rows )));
		score.contribution = integer;
		// PC�^�_���[�W
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, scoreRows[n].rows )));
		score.pcDamage = integer;
		// �����^�_���[�W
		integer = recognizeInteger ( scoreRows[n].colRange( Score::ObjectDamageOffset, scoreRows[n].cols));
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

