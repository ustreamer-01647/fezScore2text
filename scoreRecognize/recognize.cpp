/*
�L�����N�^�̃X�R�A���摜���e�L�X�g�y�ѐ��l�Ƃ��ēǂݎ��
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
	int c_rank;
	// �L�����N�^��
	std::string name;
	// ������
	enum Nationality nationality;
	// �N���X
	enum Job job;
	// �L����
	int kill;
	// �f�b�h��
	int dead;
	// �v���x
	int contribution;
	// PC�^�_���[�W
	int pcDamage;
	// �����^�_���[�W
	int objectDamage;

	Score()
	{
		int c_rank = 0;
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

// �L�����N�^�X�R�A�F��
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores )
{
	for ( size_t i = 0; i < images.size(); i++ )
	{
		struct Score score;
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

