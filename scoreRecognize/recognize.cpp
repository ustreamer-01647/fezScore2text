/*
キャラクタのスコアを，画像を基にテキスト及び数値として読み取る
*/

#include "recognize.h"

#include <iostream>
#include <sstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>

#ifdef _DEBUG
    //Debugモードの場合
    #pragma comment(lib,"opencv_core242d.lib")
    #pragma comment(lib,"opencv_highgui242d.lib")
    #pragma comment(lib,"opencv_imgproc242d.lib")
    #pragma comment(lib,"opencv_features2d242d.lib")
#else
    //Releaseモードの場合
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

// 国家
enum Nationality { NUnknown, NNetzawar, BIelsord, NHordaine, NGeburand, NCesedria };
// クラス．classが予約語なので job とする
enum Job { JUnknown, JWarrior, JScout, JSorcerer, JFencer, JCestas };

// スコア
struct Score
{
	// 順位
	int rank;
	static const int RankOffset = 0;
	static const int RankWidth = 20;
	// キャラクタ名
	std::string name;
	static const int NameOffset = 29;
	static const int NameWidth = 114;
	// 所属国
	enum Nationality nationality;
	static const int NationalityOffset = 180;
	static const int NationalityWidth = 18;
	// クラス
	enum Job job;
	static const int JobOffset = 225;
	static const int JobWidth = 84;
	// キル数
	int kill;
	static const int KillOffset = 323;
	static const int KillWidth = 46;
	// デッド数
	int dead;
	static const int DeadOffset = 386;
	static const int DeadWidth = 40;
	// 貢献度
	int contribution;
	static const int ContributionOffset = 451;
	static const int ContributionWidth = 50;
	// PC与ダメージ
	int pcDamage;
	static const int PcDamageOffset = 523;
	static const int PcDamageWidth = 88;
	// 建物与ダメージ
	int objectDamage;
	static const int ObjectDamageOffset = 629;
	//static const int ObjectDamageWidth; // 画像右端

	Score()
	{
		int rank = 0;
		name = "774";
		nationality = NUnknown;
		job = JUnknown;
		kill = 0;
		dead = 0;
		contribution = 0;
		pcDamage = 0;
		objectDamage = 0;
	}

	void dump()
	{
		std::cout << rank << ", " << name << ", " << nationality << ", " << job << ", "
			<< kill << ", " << dead << ", " <<  contribution << ", "
			<< pcDamage << ", " << objectDamage << std::endl;
	}
};

// 数字認識
int recognizeDigit ( const cv::Mat image )
{
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
	if ( image.cols * 2 == nonzero )
	{
		// 7 上端2段がすべて輝点
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

class CharactersInfo
{
private:
	// 文字幅最低要件
	static const int LowestWidth = 4;
	// 位置
	cv::vector<cv::Rect> positions;
	// 画像
	cv::Mat image;

public:
	CharactersInfo( const cv::Mat _image )
	{
		// 画像コピー
		image = _image.clone();

		// 文字存在検索
		// 輝点列検索フラグ．falseなら暗点列を探す
		bool flag = true;
		// 始点
		int left;
		// 終点
		int right;
		for ( int col = 0; col < image.cols; col++ )
		{
			// 列の輝点数を調べる
			int nonzero = cv::countNonZero( image.col(col) );
			if ( flag )
			{
				// 輝点列探索中
				if ( 0 < nonzero )
				{
					// 輝点が1個以上あったとき
					left = col;
					// フラグ切り替え
					flag = false;
				}
			}
			else
			{
				// 暗点列探索中
				if ( 0 == nonzero )
				{
					// 暗点列だったなら
					// offsetsの最後の値を参照し，幅を求める
					right = col;
					// LowestWidthを満足するか
					if ( CharactersInfo::LowestWidth <= right - left )
					{
						// 文字と認める
						cv::Mat character = image.colRange( left, right );
						// 上端輝点行を探す
						int top = 0;
						for ( int row = top; row < character.rows; row++ )
						{
							if ( 0 < cv::countNonZero( character.row(row) ) )
							{
								top = row;
								break;
							}
						}
						// 下端輝点行を探す
						int bottom = character.rows-1;
						for ( int row = bottom; row > top; row-- )
						{
							if ( 0 < cv::countNonZero( character.row(row) ) )
							{
								bottom = row+1;
								break;
							}
						}
						// 文字領域確定
						positions.push_back ( cv::Rect( left, top, right-left, bottom-top ) );
					}
					// フラグ切り替え
					flag = true;
				}
			}
		}
	}

	// 文字数
	size_t size()
	{
		return positions.size();
	}

	// 文字画像
	cv::Mat characterImage ( size_t n )
	{
		// 存在しない文字番号は原画像を返す
		if ( size() < n )
		{
			return image;
		}
		
		return image( positions[n] );
	}

	// 数字認識
	int getDigit ( size_t n )
	{
		// 文字がないときは 0
		if ( 0 == size() )
			return 0;

		// 文字出力
		//static int a = 0;
		//std::stringstream ss;
		//ss << "char" << a << ".png";
		//a++;
		//showrite( ss.str(), image( positions[n] ));

		int value = recognizeDigit ( characterImage ( n ) );
		return value;
	}

};

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

int main(int argc, char *argv[])
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

