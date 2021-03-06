/*
キャラクタのスコアを，画像を基にテキスト及び数値として読み取る
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
		// 7 上端2段がすべて輝点．ただし1ピクセルまで欠けても良い
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
	// 2値画像の輝点数を基に判別する
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

// 所属国認識
// recognizeNationalityで判断できなかった場合，カラー画像を基に判別する
enum Nationality recognizeNationality2 ( const cv::Mat image )
{
	// 閾値130.0で2値化した画像を基にする

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

// クラス認識
enum Job recognizeJob ( const cv::Mat image )
{

	// 文字列情報
	// 「リ」は2文字扱い
	CharactersInfo charactersInfo ( image );

	// 文字確認出力
	//static int counter = 0;
	//std::stringstream ss;
	//for ( size_t n = 0; n < charactersInfo.size(); n++ )
	//{
	//	ss << "char" << counter << "x" << n << ".png";
	//	showrite( ss.str(), charactersInfo.characterImage(n));
	//	ss.str("");
	//}
	//counter++;

	// 5字：ソーサラー，ウォリアー，フェンサー
	if ( 5 == charactersInfo.size() )
	{
		int second = cv::countNonZero( charactersInfo.characterImage( 1 ) );
		int fifth = cv::countNonZero( charactersInfo.characterImage( 4 ) );
		// 第2字と第5字の輝点数差が3以下なら同一文字とみなし，ソーサラー扱い
		int diff = second - fifth;
		if ( diff * diff < 3 * 3 )
		{
			return Job::JSorcerer;
		}
		// 第1字最上段がすべて輝点ならばフェンサー
		int nonzero = cv::countNonZero( charactersInfo.characterImage(0).rowRange( 0, 1 ) );
		if ( nonzero == charactersInfo.characterImage(0).cols )
		{
			return Job::JFencer;
		}
		return Job::JWarrior;
	}

	// 4字：スカウト，セスタス
	if ( 4 == charactersInfo.size() )
	{
		int second = cv::countNonZero( charactersInfo.characterImage( 1 ) );
		int fourth = cv::countNonZero( charactersInfo.characterImage( 3 ) );
		// 第2字と第4字の輝点数差が3以下なら同一文字とみなし，セスタス扱い
		int diff = second - fourth;
		if ( diff * diff < 3 * 3 )
		{
			return Job::JCestas;
		}
		return Job::JScout;
	}

	return Job::JUnknown;
}

// キャラクタスコア認識
// 使わない
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

/** キャラクタスコア認識
@param ss fezのスクリーンショット
@param scores 認識したスコアデータ入力先
*/
void recognize ( const cv::Mat ss, std::vector<struct Score> &scores )
{
	// 切り出し
	cv::Mat scoreTable = extractScoreTable( ss );

	// 2値画像を作る
	cv::Mat scoreTableBinary = scoreTable2Binary( scoreTable );
	//showrite( "table.png", scoreTableBinary );

	// キャラクタスコア切り出し
	cv::vector<cv::Mat> scoreRows;
	extractScoreRows( scoreTableBinary, scoreRows );

	for ( size_t n = 0; n < scoreRows.size(); n++ )
	{
		struct Score score;
		int integer = 0;
		enum Nationality nationality = Nationality::NUnknown;
		enum Job job = Job::JUnknown;

		// 順位
		integer = recognizeInteger( scoreRows[n]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, scoreRows[n].rows )));
		score.rank = integer;
		// キャラクタ名
		//recognizeText ( scoreRows[n], Score::NameOffset, Score::NameWidth );
		// 所属国
		nationality = recognizeNationality ( scoreRows[n] ( cv::Rect( Score::NationalityOffset, 0, Score::NationalityWidth, scoreRows[n].rows )));
		if ( nationality == NUnknown )
		{
			nationality = recognizeNationality2( extractColorNationality( scoreTable, n) );
		}
		score.nationality = nationality;
		// クラス
		job = recognizeJob ( scoreRows[n]( cv::Rect (Score::JobOffset, 0, Score::JobWidth, scoreRows[n].rows )));
		score.job = job;
		// キル数
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, scoreRows[n].rows )));
		score.kill = integer;
		// デッド数
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, scoreRows[n].rows )));
		score.dead = integer;
		// 貢献度
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, scoreRows[n].rows )));
		score.contribution = integer;
		// PC与ダメージ
		integer = recognizeInteger ( scoreRows[n]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, scoreRows[n].rows )));
		score.pcDamage = integer;
		// 建物与ダメージ
		integer = recognizeInteger ( scoreRows[n].colRange( Score::ObjectDamageOffset, scoreRows[n].cols));
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

