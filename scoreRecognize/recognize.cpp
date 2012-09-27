/*
�L�����N�^�̃X�R�A���C�摜����Ƀe�L�X�g�y�ѐ��l�Ƃ��ēǂݎ��
*/

#include "recognize.h"

#include <iostream>
#include <sstream>

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/features2d/features2d.hpp>

#ifdef _DEBUG
    //Debug���[�h�̏ꍇ
    #pragma comment(lib,"opencv_core242d.lib")
    #pragma comment(lib,"opencv_highgui242d.lib")
    #pragma comment(lib,"opencv_imgproc242d.lib")
    #pragma comment(lib,"opencv_features2d242d.lib")
#else
    //Release���[�h�̏ꍇ
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

// ����
enum Nationality { NUnknown, NNetzawar, BIelsord, NHordaine, NGeburand, NCesedria };
// �N���X�Dclass���\���Ȃ̂� job �Ƃ���
enum Job { JUnknown, JWarrior, JScout, JSorcerer, JFencer, JCestas };

// �X�R�A
struct Score
{
	// ����
	int rank;
	static const int RankOffset = 0;
	static const int RankWidth = 20;
	// �L�����N�^��
	std::string name;
	static const int NameOffset = 29;
	static const int NameWidth = 114;
	// ������
	enum Nationality nationality;
	static const int NationalityOffset = 180;
	static const int NationalityWidth = 18;
	// �N���X
	enum Job job;
	static const int JobOffset = 225;
	static const int JobWidth = 84;
	// �L����
	int kill;
	static const int KillOffset = 323;
	static const int KillWidth = 46;
	// �f�b�h��
	int dead;
	static const int DeadOffset = 386;
	static const int DeadWidth = 40;
	// �v���x
	int contribution;
	static const int ContributionOffset = 451;
	static const int ContributionWidth = 50;
	// PC�^�_���[�W
	int pcDamage;
	static const int PcDamageOffset = 523;
	static const int PcDamageWidth = 88;
	// �����^�_���[�W
	int objectDamage;
	static const int ObjectDamageOffset = 629;
	//static const int ObjectDamageWidth; // �摜�E�[

	Score()
	{
		int rank = 0;
		name = "774";
		nationality = NUnknown;
		job = JUnknown;
		kill = 0;
		dead = 0;
		contribution = 0;
		pcDamage = 0;
		objectDamage = 0;
	}

	void dump()
	{
		std::cout << rank << ", " << name << ", " << nationality << ", " << job << ", "
			<< kill << ", " << dead << ", " <<  contribution << ", "
			<< pcDamage << ", " << objectDamage << std::endl;
	}
};

// �����F��
int recognizeDigit ( const cv::Mat image )
{
	int nonzero = 0;

	nonzero = cv::countNonZero( image.colRange( image.cols-2, image.cols ));
	if ( image.rows * 2 == nonzero )
	{
		// 1 �E�[2�񂪂��ׂċP�_
		return 1;
	}

	nonzero = cv::countNonZero( image.rowRange( image.rows-2, image.rows ));
	if ( image.cols * 2 == nonzero )
	{
		// 2 ���[2�i�����ׂċP�_
		return 2;
	}

	nonzero = cv::countNonZero( image.rowRange( 0, 2 ));
	if ( image.cols * 2 == nonzero )
	{
		// 7 ��[2�i�����ׂċP�_
		return 7;
	}

	nonzero = cv::countNonZero( image.colRange( image.cols-3, image.cols-1 ));
	if ( image.rows * 2 == nonzero )
	{
		// 4 �E�[�̍�2�񂪂��ׂċP�_
		return 4;
	}

	nonzero = cv::countNonZero( image( cv::Rect( 0, 0, 1, image.rows*2/3 ) ));
	if ( 0 == nonzero )
	{
		// 3 ���[�̏㕔3����2�����ׂĈÓ_
		return 3;
	}

	nonzero = cv::countNonZero( image( cv::Rect( 0, image.rows/2, 3, 2 ) ));
	if ( 0 == nonzero )
	{
		// ���[�̉������J�n������3x2�����ׂĈÓ_
		return 5;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols/2, image.rows/2-1, 1, 3 ) ));
	if ( 0 == nonzero )
	{
		// �����񒆉�3�s�N�Z�������ׂĈÓ_
		return 0;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols-1, 0, 1, image.rows*2/5 ) ));
	if ( 0 == nonzero )
	{
		// �E�[�㕔5����2�����ׂĈÓ_
		return 6;
	}

	nonzero = cv::countNonZero( image( cv::Rect( image.cols-1, image.rows-3, 1, 3 ) ));
	if ( 0 == nonzero )
	{
		// �E�[����3�s�N�Z�������ׂĈÓ_
		return 9;
	}

	// ��L�����ɓ��Ă͂܂�Ȃ��ꍇ
	return 8;
}

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
	CharactersInfo( const cv::Mat _image )
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

		// �����o��
		//static int a = 0;
		//std::stringstream ss;
		//ss << "char" << a << ".png";
		//a++;
		//showrite( ss.str(), image( positions[n] ));

		int value = recognizeDigit ( characterImage ( n ) );
		return value;
	}

};

// �摜���琔�l��ǂݏo��
int recognizeInteger ( const cv::Mat image )
{
	// ��������
	CharactersInfo charactersInfo( image );

	// ���l�F��
	std::stringstream ss;
	for ( size_t n = 0; n < charactersInfo.size(); n++ )
	{
		ss << charactersInfo.getDigit ( n );
	}

	int integer;
	ss >> integer;
	
	return integer;
}

// �L�����N�^�X�R�A�F��
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores )
{
	for ( size_t i = 0; i < images.size(); i++ )
	{
		struct Score score;
		int integer = 0;
		// ����
		integer = recognizeInteger( images[i]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, images[i].rows )));
		score.rank = integer;
		// �L�����N�^��
		//recognizeText ( images[i], Score::NameOffset, Score::NameWidth );
		// ������
		//recognizeNationalityIcon ( images[i], Score::NationalityOffset, Score::NationalityWidth );
		// �N���X
		//recognizeText ( images[i], Score::JobOffset, Score::JobWidth );
		// �L����
		integer = recognizeInteger ( images[i]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, images[i].rows )));
		score.kill = integer;
		// �f�b�h��
		integer = recognizeInteger ( images[i]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, images[i].rows )));
		score.dead = integer;
		// �v���x
		integer = recognizeInteger ( images[i]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, images[i].rows )));
		score.contribution = integer;
		// PC�^�_���[�W
		integer = recognizeInteger ( images[i]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, images[i].rows )));
		score.pcDamage = integer;
		// �����^�_���[�W
		integer = recognizeInteger ( images[i].colRange( Score::ObjectDamageOffset, images[i].cols));
		score.objectDamage = integer;

		score.dump();
		scores.push_back ( score );
	}

}

int main(int argc, char *argv[])
{
	// �e�X�g�摜�I��
	std::vector<std::string> testFiles;
	testFiles.push_back( "300scoreRow.png" );
	testFiles.push_back( "301scoreRow.png" );
	testFiles.push_back( "302scoreRow.png" );
	testFiles.push_back( "303scoreRow.png" );
	testFiles.push_back( "304scoreRow.png" );
	testFiles.push_back( "305scoreRow.png" );
	testFiles.push_back( "306scoreRow.png" );
	testFiles.push_back( "307scoreRow.png" );
	testFiles.push_back( "308scoreRow.png" );
	testFiles.push_back( "309scoreRow.png" );
	// �摜�ǂݍ���
	cv::vector<cv::Mat> testImages;
	for ( size_t i = 0; i <testFiles.size(); i++ )
	{
		cv::Mat image = cv::imread( testFiles[i], 0 );
		testImages.push_back  ( image );
	}

	// �X�R�A�F��
	std::vector<struct Score> scores;
	recognize( testImages, scores );


	return 0;
}

