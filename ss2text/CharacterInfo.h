#pragma once

#include <opencv2/core/core.hpp>
#include "recognize.h"

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
	CharactersInfo( const cv::Mat _image );

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

		int value = recognizeDigit ( characterImage ( n ) );
		return value;
	}

};

