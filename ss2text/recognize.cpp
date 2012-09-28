/*
キャラクタのスコアを，画像を基にテキスト及び数値として読み取る
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

// 数字認識
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
		// 1 右端2列がすべて輝点
		return 1;
	}

	nonzero = cv::countNonZero( image.rowRange( image.rows-2, image.rows ));
	if ( image.cols * 2 == nonzero )
	{
		// 2 下端2段がすべて輝点
		return 2;
	}

	nonzero = cv::countNonZero( image.rowRange( 0, 2 ));
	if ( image.cols * 2 - 2 < nonzero )
	{
		// 7 上端2段がすべて輝点．ただし2ピクセルまで欠けても良い
		return 7;
	}

	nonzero = cv::countNonZero( image.colRange( image.cols-3, image.cols-1 ));
	if ( image.rows * 2 == nonzero )
	{
		// 4 右端の左2列がすべて輝点
		return 4;
	}

	nonzero = cv::countNonZero( image( cv::Rect( 0, 0, 1, image.rows*2/3 ) ));
	if ( 0 == nonzero )
	{
		// 3 左端の上部3分の2がすべて暗点
		return 3;
	}

	nonzero = cv::countNonZero( image( cv::Rect( 0, image.rows/2, 3, 2 ) ));
	if ( 0 == nonzero )
	{
		// 左端の下半分開始すぐの3x2がすべて暗点
		return 5;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols/2, image.rows/2-1, 1, 3 ) ));
	if ( 0 == nonzero )
	{
		// 中央列中央3ピクセルがすべて暗点
		return 0;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols-1, 0, 1, image.rows*2/5 ) ));
	if ( 0 == nonzero )
	{
		// 右端上部5分の2がすべて暗点
		return 6;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols-1, image.rows-3, 1, 3 ) ));
	if ( 0 == nonzero )
	{
		// 右端下部3ピクセルがすべて暗点
		return 9;
	}

	// 上記条件に当てはまらない場合
	return 8;
}

// 所属国認識
enum Nationality recognizeNationality ( const cv::Mat image )
{
	// yellow hor 40
	// green ces 80
	// blue iel 160
	// purple geb 200
	// red net 



	return Nationality::NUnknown;
}

// 画像から数値を読み出す
int recognizeInteger ( const cv::Mat image )
{
	// 文字列情報
	CharactersInfo charactersInfo( image );

	// 数値認識
	std::stringstream ss;
	for ( size_t n = 0; n < charactersInfo.size(); n++ )
	{
		ss << charactersInfo.getDigit ( n );
	}

	int integer;
	ss >> integer;
	
	return integer;
}

// キャラクタスコア認識
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores )
{
	for ( size_t i = 0; i < images.size(); i++ )
	{
		struct Score score;
		int integer = 0;
		// 順位
		integer = recognizeInteger( images[i]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, images[i].rows )));
		score.rank = integer;
		// キャラクタ名
		//recognizeText ( images[i], Score::NameOffset, Score::NameWidth );
		// 所属国
		//recognizeNationalityIcon ( images[i], Score::NationalityOffset, Score::NationalityWidth );
		// クラス
		//recognizeText ( images[i], Score::JobOffset, Score::JobWidth );
		// キル数
		integer = recognizeInteger ( images[i]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, images[i].rows )));
		score.kill = integer;
		// デッド数
		integer = recognizeInteger ( images[i]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, images[i].rows )));
		score.dead = integer;
		// 貢献度
		integer = recognizeInteger ( images[i]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, images[i].rows )));
		score.contribution = integer;
		// PC与ダメージ
		integer = recognizeInteger ( images[i]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, images[i].rows )));
		score.pcDamage = integer;
		// 建物与ダメージ
		integer = recognizeInteger ( images[i].colRange( Score::ObjectDamageOffset, images[i].cols));
		score.objectDamage = integer;

		score.dump();
		scores.push_back ( score );
	}

}

void recognize ( const cv::Mat scoreTable, const cv::Mat scoreTableBinary, std::vector<struct Score> &scores )
{
	// カラー画像
	cv::vector<cv::Mat> scoreRows;
	extractScoreRows( scoreTable, scoreRows );
	
	// 2値画像
	cv::vector<cv::Mat> scoreRowsBinary;
	extractScoreRows( scoreTableBinary, scoreRowsBinary );

	for ( size_t n = 0; n < scoreRows.size(); n++ )
	{
		struct Score score;
		int integer = 0;
		enum Nationality nationality = Nationality::NUnknown;
		enum Job job = Job::JUnknown;

		// 順位
		integer = recognizeInteger( scoreRowsBinary[n]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, scoreRowsBinary[n].rows )));
		score.rank = integer;
		// キャラクタ名
		//recognizeText ( scoreRowsBinary[n], Score::NameOffset, Score::NameWidth );
		// 所属国
		//nationality = recognizeNationalityIcon ( scoreRows[n], Score::NationalityOffset, Score::NationalityWidth );
		//score.nationality = nationality;
		// クラス
		//recognizeText ( scoreRowsBinary[n], Score::JobOffset, Score::JobWidth );
		// キル数
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, scoreRowsBinary[n].rows )));
		score.kill = integer;
		// デッド数
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, scoreRowsBinary[n].rows )));
		score.dead = integer;
		// 貢献度
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, scoreRowsBinary[n].rows )));
		score.contribution = integer;
		// PC与ダメージ
		integer = recognizeInteger ( scoreRowsBinary[n]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, scoreRowsBinary[n].rows )));
		score.pcDamage = integer;
		// 建物与ダメージ
		integer = recognizeInteger ( scoreRowsBinary[n].colRange( Score::ObjectDamageOffset, scoreRowsBinary[n].cols));
		score.objectDamage = integer;

		score.dump();
		scores.push_back ( score );
	}
}

static int testmain(int argc, char *argv[])
{
	// テスト画像選択
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
	// 画像読み込み
	cv::vector<cv::Mat> testImages;
	for ( size_t i = 0; i <testFiles.size(); i++ )
	{
		cv::Mat image = cv::imread( testFiles[i], 0 );
		testImages.push_back  ( image );
	}

	// スコア認識
	std::vector<struct Score> scores;
	recognize( testImages, scores );


	return 0;
}

