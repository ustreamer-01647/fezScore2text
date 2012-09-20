/*
キャラクタのスコアを画像をテキスト及び数値として読み取る
*/

#include "recognize.h"

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
	int c_rank;
	// キャラクタ名
	std::string name;
	// 所属国
	enum Nationality nationality;
	// クラス
	enum Job job;
	// キル数
	int kill;
	// デッド数
	int dead;
	// 貢献度
	int contribution;
	// PC与ダメージ
	int pcDamage;
	// 建物与ダメージ
	int objectDamage;

	Score()
	{
		int c_rank = 0;
		name = "774";
		nationality = NUnknown;
		job = JUnknown;
		kill = 0;
		dead = 0;
		contribution = 0;
		pcDamage = 0;
		objectDamage = 0;
	}
};

// キャラクタスコア認識
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores )
{
	for ( size_t i = 0; i < images.size(); i++ )
	{
		struct Score score;
		scores.push_back ( score );
	}

}

int main(int argc, char *argv[])
{
	// テスト画像選択
	std::vector<std::string> testFiles;
	testFiles.push_back( "1.png" );
	testFiles.push_back( "5.png" );
	testFiles.push_back( "player.png" );
	// 画像読み込み
	cv::vector<cv::Mat> testImages;
	for ( size_t i = 0; i <testFiles.size(); i++ )
	{
		cv::Mat image = cv::imread( testFiles[i] );
		testImages.push_back  ( image );
	}

	// スコア認識
	std::vector<struct Score> scores;
	for ( size_t i = 0; i < testImages.size(); i++ )
	{
		recognize( testImages, scores );
	}

	return 0;
}

