using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using OpenCvSharp;

namespace ss2textCS
{
    partial class Program
    {

        // extractScoreTable() で使用する
        const int ExtractOffsetX = 362;
        const int ExtractOffsetY = 176;
        const int TableWidth = 720;
        const int TableHeight = 352;

        // SSからスコア表を抽出
        CvMat extractScoreTable ( CvMat ss )
        {
            // 画像中央から一定位置の一定矩形
            CvRect rect = new CvRect( 
                ss.Cols / 2 - ExtractOffsetX,
                ss.Rows / 2 - ExtractOffsetY,
                TableWidth, TableHeight);

            CvMat table;
            ss.GetSubArr ( out table, rect );
            return table;
        }

        // extractRows() で使用する
        const int ScoreRowHeight = 17;
        const int ScoreRowInterval = 15;
        const int ScoreTop10Rows = 10;

        // n番目アイコン部分を返す
        CvMat extractColorNationality ( CvMat scoreTable, int n )
        {
            CvRect rect;
            CvMat scoreRow;

            if ( n < 10 )
            {
                // ランキング部分から抽出
                rect = new CvRect( 
                    Score.NationalityOffset, n * ( ScoreRowHeight + ScoreRowInterval ),
                    Score.NationalityWidth, ScoreRowHeight ) );
                
            }
            else
            {
                // プレイヤーキャラクタスコアを抽出
                rect = new CvRect( 
                    Score.NationalityOffset, scoreTable.Rows - ScoreRowHeight,
                    Score.NationalityWidth, ScoreRowHeight );
            }

            scoreTable.GetSubArr ( out scoreRow, rect );
            return scoreRow;
        }

        // スコア表からキャラクタ単位に切り分ける
        void extractScoreRows ( CvMat scoreTable, ref List<CvMat> scoreRows )
        {
            CvMat scoreRow;
            CvRect rect;
            // ランキングスコア10件
            for ( int i = 0; i < ScoreTop10Rows; i++ )
            {
                rect = new CvRect ( 0, i * ( ScoreRowHeight + ScoreRowInterval ),
                    scoreTable.Cols, ScoreRowHeight );
                scoreTable.GetSubArr ( out scoreRow, rect );
                
                scoreRows.Add( scoreRow );
            }

            // プレイヤースコア
            rect = new CvRect ( 0, scoreTable.Rows - ScoreRowHeight, scoreTable.Cols, ScoreRowHeight );
            scoreTable.GetSubArr ( out scoreRow, rect );

            scoreRows.Add ( scoreRow );
        }

        // 孤立輝点除去
        // 周囲に輝点が無い場合，その輝点を消す
        CvMat removeNoize ( CvMat image )
        {
            CvMat workImage;
            image.CopyMakeBorder( workImage, new CvPoint(1, 1), BorderType.Constant );
        }
}

// 孤立輝点除去
// 周囲に輝点がない場合，その輝点を消す
cv::Mat removeNoise ( const cv::Mat image )
{
	// 一回り大きい行列
	cv::Mat workImage = cv::Mat::zeros( image.rows+2, image.cols+2, image.type() );
	// 作業用行列にimageを入力する
	image.copyTo( workImage( cv::Rect( 1, 1, image.cols, image.rows ) ));
	//showrite( "21workImage.png", workImage );

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
	//showrite( "22workImage.png", workImage );

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

}
}
