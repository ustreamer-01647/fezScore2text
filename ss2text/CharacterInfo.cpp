#include "CharacterInfo.h"

CharactersInfo::CharactersInfo( const cv::Mat _image )
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
