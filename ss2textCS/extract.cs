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
        static CvMat extractScoreTable(CvMat ss)
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
        static CvMat extractColorNationality(CvMat scoreTable, int n)
        {
            CvRect rect;
            CvMat scoreRow;

            if ( n < 10 )
            {
                // ランキング部分から抽出
                rect = new CvRect( 
                    Score.NationalityOffset, n * ( ScoreRowHeight + ScoreRowInterval ),
                    Score.NationalityWidth, ScoreRowHeight );
                
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
        static void extractScoreRows(CvMat scoreTable, ref List<CvMat> scoreRows)
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
        static CvMat removeNoize(CvMat image)
        {
            // 1px大きい作業用画像
            CvMat workImage = new CvMat( image.Rows+1, image.Cols+1, MatrixType.U8C1 );
            image.CopyMakeBorder( workImage, new CvPoint(1, 1), BorderType.Constant );

            // 走査
            for ( int row = 0; row < image.Rows; row++ )
            {
                for ( int col = 0; col < image.Cols; col++ )
                {
                    // 注目画素が暗点ならば何もしない
                    if ( 0 == image.Get2D( row, col ))
                        continue;

                    // 範囲3x3の輝点が1ならば，中心画素を暗点にする
                    CvRect rect = new CvRect( col, row, 3, 3 );
                    CvMat area;
                    workImage.GetSubArr ( out area, rect );
                    int nonzero = area.CountNonZero();
                    if ( 1 == nonzero )
                        image.Set2D( row, col, 0 );
                }
            }
            return image;
        }

        // スコア表画像を2値化
        static CvMat scoreTable2Binary(CvMat scoreTable)
        {
            CvMat bin = new CvMat( scoreTable.Rows, scoreTable.Cols, MatrixType.U8C1);
            // グレイスケール化
            scoreTable.CvtColor( bin, ColorConversion.BgrToGray );
            // 2値化
            bin.Threshold ( bin, 150.0, 255.0, ThresholdType.Binary );
            // 孤立輝点除去
            bin = removeNoize ( bin );
            
            return bin;
        }

    }

}
