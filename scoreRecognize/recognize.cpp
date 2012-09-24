/*
�L�����N�^�̃X�R�A���C�摜����Ƀe�L�X�g�y�ѐ��l�Ƃ��ēǂݎ��
*/

#include "recognize.h"

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
	static const int RankWidth = 14;
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
};

// �����F��
int recognizeNumeric ( const cv::Mat image )
{
	/*
	1 �E�[2�񂪂��ׂċP�_
	2 ���[2�i�����ׂċP�_
	7 ��[2�i�����ׂċP�_
	4 �E�[�̍�2�񂪂��ׂċP�_
	�֊s���o
	8 �֊s��3��
	3, 5 �֊s��1��
		3 �E������Ó_
		5 �E������P�_
	0 6 9 �֊s��2��
		6 ��2�֊s����3����1�ɂȂ�
		0 �����񂪋P�_�C�Ó_�C�P�_
		9 ��L�ȊO
	*/

	if ( image.rows * 2 == cv::countNonZero( image.colRange( image.cols-2, image.cols )) )
	{
		// 1 �E�[2�񂪂��ׂċP�_
		return 1;
	}

	if ( image.cols * 2 == cv::countNonZero( image.rowRange( image.rows-2, image.rows )) )
	{
		// 2 ���[2�i�����ׂċP�_
		return 2;
	}

	if ( image.cols * 2 == cv::countNonZero( image.rowRange( 0, 2 )) )
	{
		// 7 ��[2�i�����ׂċP�_
		return 7;
	}
	if ( image.rows * 2 == cv::countNonZero( image.colRange( image.cols-3, image.cols-1 )) )
	{
		// 4 �E�[�̍�2�񂪂��ׂċP�_
		return 4;
	}

	// �֊s���o
	// �֊s���
	std::vector<std::vector<cv::Point> > contours;
	// �K�w�\��
	std::vector<cv::Vec4i> hierarchy;
	// 2�l�摜�C�֊s�i�o�́j�C�K�w�\���i�o�́j�C�֊s���o���[�h�C�֊s�̋ߎ���@
	cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

	if ( 3 == hierarchy.size() )
	{
		// �֊s��3
		return 8;
	}

	if ( 2 == hierarchy.size() )
	{
		// �֊s��2�D0, 6, 9�̂����ꂩ
		// ��2�֊s�n�_���㕔3����1�ɂȂ��Ƃ�6
		if ( contours[1][0].y > (image.rows/3) )
		{	
			return 6;		
		}
	}



	return 0;
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
	int getNumeric ( size_t n )
	{
		// �������Ȃ��Ƃ��� 0
		if ( 0 == size() )
			return 0;

		static int a = 0;
		std::stringstream ss;
		ss << "char" << a << ".png";
		a++;
		showrite( ss.str(), image( positions[n] ));

		return recognizeNumeric ( characterImage ( n ) );
	}

};

// �摜���琔�l��ǂݏo��
int recognizeInteger ( const cv::Mat image )
{
	// ��������
	CharactersInfo charactersInfo( image );

	// ���l�F��
	std::stringstream integer;
	for ( size_t n = 0; n < charactersInfo.size(); n++ )
	{
		integer << charactersInfo.getNumeric ( n );
	}
	
	return 0;
}

// �L�����N�^�X�R�A�F��
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores )
{
	for ( size_t i = 0; i < images.size(); i++ )
	{
		struct Score score;
		
		// ����
		recognizeInteger( images[i]( cv::Rect ( Score::RankOffset, 0, Score::RankWidth, images[i].rows )));
		// �L�����N�^��
		//recognizeText ( images[i], Score::NameOffset, Score::NameWidth );
		// ������
		//recognizeNationalityIcon ( images[i], Score::NationalityOffset, Score::NationalityWidth );
		// �N���X
		//recognizeText ( images[i], Score::JobOffset, Score::JobWidth );
		// �L����
		recognizeInteger ( images[i]( cv::Rect( Score::KillOffset, 0, Score::KillWidth, images[i].rows )));
		// �f�b�h��
		recognizeInteger ( images[i]( cv::Rect( Score::DeadOffset, 0, Score::DeadWidth, images[i].rows )));
		// �v���x
		recognizeInteger ( images[i]( cv::Rect( Score::ContributionOffset, 0, Score::ContributionWidth, images[i].rows )));
		// PC�^�_���[�W
		recognizeInteger ( images[i]( cv::Rect( Score::PcDamageOffset, 0, Score::PcDamageWidth, images[i].rows )));
		// �����^�_���[�W
		recognizeInteger ( images[i].colRange( Score::ObjectDamageOffset, images[i].cols));

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

