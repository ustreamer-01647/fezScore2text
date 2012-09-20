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

class CharactersInfo
{
private:
	// �������Œ�v��
	static const int LowestWidth = 4;
	// �ʒu
	std::vector<int> offsets;
	// ��
	std::vector<int> widths;
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
					offsets.push_back(col);
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
					int width = col - offsets.back();
					// LowestWidth�𖞑����邩
					if ( CharactersInfo::LowestWidth <= width )
					{
						// �����ƔF�߂�
						widths.push_back( width );
					}
					else
					{
						// �����Ƃ͔F�߂Ȃ�
						// ���O��offsets����������
						offsets.pop_back();
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
		return offsets.size();
	}

	// �����摜
	cv::Mat characterImage ( size_t n )
	{
		// ���݂��Ȃ������ԍ��͌��摜��Ԃ�
		if ( size() < n )
		{
			return image;
		}
		
		return image( cv::Rect( offsets[n], 0, widths[n], image.rows ) );
	}

};

// �摜���琔�l��ǂݏo��
int recognizeNumeric ( const cv::Mat image, const int offset, const int width )
{
	// ��������
	std::vector<struct CharactersInfo> charactersInfo( image );;

	// ���l�F��
	// �A��

	return 0;
}

// �L�����N�^�X�R�A�F��
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores )
{
	for ( size_t i = 0; i < images.size(); i++ )
	{
		struct Score score;
		
		// ����
		recognizeNumeric( images[i], Score::RankOffset, Score::RankWidth );
		// �L�����N�^��
		//recognizeText ( images[i], Score::NameOffset, Score::NameWidth );
		// ������
		//recognizeNationalityIcon ( images[i], Score::NationalityOffset, Score::NationalityWidth );
		// �N���X
		//recognizeText ( images[i], Score::JobOffset, Score::JobWidth );
		// �L����
		recognizeNumeric ( images[i], Score::KillOffset, Score::KillWidth );
		// �f�b�h��
		recognizeNumeric ( images[i], Score::DeadOffset, Score::DeadWidth );
		// �v���x
		recognizeNumeric ( images[i], Score::ContributionOffset, Score::ContributionWidth );
		// PC�^�_���[�W
		recognizeNumeric ( images[i], Score::PcDamageOffset, Score::PcDamageWidth );
		// �����^�_���[�W
		recognizeNumeric ( images[i], Score::ObjectDamageOffset, images[i].cols - Score::ObjectDamageOffset );

		scores.push_back ( score );
	}

}

int main(int argc, char *argv[])
{
	// �e�X�g�摜�I��
	std::vector<std::string> testFiles;
	testFiles.push_back( "1.png" );
	testFiles.push_back( "5.png" );
	testFiles.push_back( "player.png" );
	// �摜�ǂݍ���
	cv::vector<cv::Mat> testImages;
	for ( size_t i = 0; i <testFiles.size(); i++ )
	{
		cv::Mat image = cv::imread( testFiles[i] );
		testImages.push_back  ( image );
	}

	// �X�R�A�F��
	std::vector<struct Score> scores;
	for ( size_t i = 0; i < testImages.size(); i++ )
	{
		recognize( testImages, scores );
	}

	return 0;
}

