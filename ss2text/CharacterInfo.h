#pragma once

#include <opencv2/core/core.hpp>
#include "recognize.h"

class CharactersInfo
{
private:
	// �������Œ�v��
	static const int LowestWidth = 4;
	// �ʒu
	cv::vector<cv::Rect> positions;
	// �摜
	cv::Mat image;

public:
	CharactersInfo( const cv::Mat _image );

	// ������
	size_t size()
	{
		return positions.size();
	}

	// �����摜
	cv::Mat characterImage ( size_t n )
	{
		// ���݂��Ȃ������ԍ��͌��摜��Ԃ�
		if ( size() < n )
		{
			return image;
		}
		
		return image( positions[n] );
	}

	// �����F��
	int getDigit ( size_t n )
	{
		// �������Ȃ��Ƃ��� 0
		if ( 0 == size() )
			return 0;

		int value = recognizeDigit ( characterImage ( n ) );
		return value;
	}

};

