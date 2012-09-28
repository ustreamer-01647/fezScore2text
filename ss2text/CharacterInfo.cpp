#include "CharacterInfo.h"

CharactersInfo::CharactersInfo( const cv::Mat _image )
{
	// �摜�R�s�[
	image = _image.clone();

	// �������݌���
	// �P�_�񌟍��t���O�Dfalse�Ȃ�Ó_���T��
	bool flag = true;
	// �n�_
	int left;
	// �I�_
	int right;
	for ( int col = 0; col < image.cols; col++ )
	{
		// ��̋P�_���𒲂ׂ�
		int nonzero = cv::countNonZero( image.col(col) );
		if ( flag )
		{
			// �P�_��T����
			if ( 0 < nonzero )
			{
				// �P�_��1�ȏ゠�����Ƃ�
				left = col;
				// �t���O�؂�ւ�
				flag = false;
			}
		}
		else
		{
			// �Ó_��T����
			if ( 0 == nonzero )
			{
				// �Ó_�񂾂����Ȃ�
				// offsets�̍Ō�̒l���Q�Ƃ��C�������߂�
				right = col;
				// LowestWidth�𖞑����邩
				if ( CharactersInfo::LowestWidth <= right - left )
				{
					// �����ƔF�߂�
					cv::Mat character = image.colRange( left, right );
					// ��[�P�_�s��T��
					int top = 0;
					for ( int row = top; row < character.rows; row++ )
					{
						if ( 0 < cv::countNonZero( character.row(row) ) )
						{
							top = row;
							break;
						}
					}
					// ���[�P�_�s��T��
					int bottom = character.rows-1;
					for ( int row = bottom; row > top; row-- )
					{
						if ( 0 < cv::countNonZero( character.row(row) ) )
						{
							bottom = row+1;
							break;
						}
					}
					// �����̈�m��
					positions.push_back ( cv::Rect( left, top, right-left, bottom-top ) );
				}
				// �t���O�؂�ւ�
				flag = true;
			}
		}
	}
}
