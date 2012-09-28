#include <sstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>

#include "extract.h"


static void showrite(const std::string s, cv::Mat &image)
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

// 孤立輝点除去
// 周囲に輝点がない場合，その輝点を消す
cv::Mat removeNoise ( const cv::Mat image )
{
	// 一回り大きい行列
	cv::Mat workImage = cv::Mat::zeros( image.rows+2, image.cols+2, image.type() );
	// 作業用行列にimageを入力する
	image.copyTo( workImage( cv::Rect( 1, 1, image.cols, image.rows ) ));
	showrite( "21workImage.png", workImage );

	// 走査
	for ( int row = 0; row < image.rows; row++ )
	{
		for ( int col = 0; col < image.cols; col++ )
		{
			// 注目画素が暗点ならば，何もしない
			if ( 0 == image.at<unsigned char>( row, col ) )
				continue;

			// 範囲 3x3 の輝点が1ならば，中心画素を暗点にする
			int nonzero = cv::countNonZero( workImage( cv::Rect ( col, row, 3, 3 ) ));
			if ( 1 == nonzero )
				workImage.at<unsigned char>( row+1, col+1 ) = 0;
		}
	}
	showrite( "22workImage.png", workImage );

	return workImage( cv::Rect( 1, 1, image.cols, image.rows ) );
}

// スコア表画像を2値化する
cv::Mat scoreTable2Binary( const cv::Mat scoreTable )
{
	cv::Mat bin;

	// グレイスケール化
	cv::cvtColor( scoreTable, bin, CV_BGR2GRAY );
	// 2値化
	cv::threshold( bin, bin, 150.0, 255.0, CV_THRESH_BINARY );
	// 孤立輝点除去
	bin = removeNoise ( bin );

	return bin;
}

static int testmain(int argc, char *argv[])
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

	// 孤立輝点除去
	cv::Mat nonNoise;
	nonNoise = removeNoise( bin );
	showrite( "25nonNoise.png", nonNoise );

	// 行抽出
	cv::vector<cv::Mat> scoreRows;
	extractScoreRows( nonNoise, scoreRows );

	cv::waitKey();

	return 0;
}
