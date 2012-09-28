#pragma once

#include <opencv2/core/core.hpp>
#include "recognize.h"

class CharactersInfo
{
private:
	// •¶š•Å’á—vŒ
	static const int LowestWidth = 4;
	// ˆÊ’u
	cv::vector<cv::Rect> positions;
	// ‰æ‘œ
	cv::Mat image;

public:
	CharactersInfo( const cv::Mat _image );

	// •¶š”
	size_t size()
	{
		return positions.size();
	}

	// •¶š‰æ‘œ
	cv::Mat characterImage ( size_t n )
	{
		// ‘¶İ‚µ‚È‚¢•¶š”Ô†‚ÍŒ´‰æ‘œ‚ğ•Ô‚·
		if ( size() < n )
		{
			return image;
		}
		
		return image( positions[n] );
	}

	// ”š”F¯
	int getDigit ( size_t n )
	{
		// •¶š‚ª‚È‚¢‚Æ‚«‚Í 0
		if ( 0 == size() )
			return 0;

		int value = recognizeDigit ( characterImage ( n ) );
		return value;
	}

};

