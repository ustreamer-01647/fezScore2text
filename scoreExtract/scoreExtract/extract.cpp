#include "extract.h"

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

// extractScoreTable() で使用する
const int ExtractOffsetX = 362;
const int ExtractOffsetY = 176;
const int TableWidth = 720;
const int TableHeight = 352;

cv::Mat extractScoreTable ( const cv::Mat ss )
{
	// 画像中央から一定オフセット位置の一定矩形
	return ss( cv::Rect( 
		ss.cols / 2 - ExtractOffsetX,
		ss.rows / 2 - ExtractOffsetY,
		TableWidth, TableHeight));
}

// extractRows() で使用する
const int ScoreRowHeight = 17;
const int ScoreRowInterval = 15;
const int ScoreTop10Rows = 10;

void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows )
{
	// ランキングスコア10件
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

	// 自キャラクタスコア
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
	 1280x800 では xoffset 278 yoffset 224 
	 画像中央から
	 左に 15:38 (paulga) 362 [1280/2-278]
	 上に 15:42 (paulga) 176 [800/2-224]
	 移動した位置を始点とする，前述したサイズの矩形に注目する
	*/


	// ランキング表部分を抽出する
	cv::Mat scoreTable = extractScoreTable( ss );
	showrite("10scoreTable.png", scoreTable);

	// 各キャラクタスコア行を抽出する
	
	// グレイスケール化
	cv::Mat gray;
	cv::cvtColor( scoreTable, gray, CV_BGR2GRAY );
	showrite( "15grayscale.png", gray );

	// 2値化
	cv::Mat bin;
	cv::threshold( gray, bin, 150.0, 255.0, CV_THRESH_BINARY );
	showrite( "20binary.png", bin );

	// 行抽出
	cv::vector<cv::Mat> scoreRows;
	extractScoreRows( bin, scoreRows );

	cv::waitKey();

	return 0;
}

