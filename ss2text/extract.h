#pragma once

#include <opencv2/core/core.hpp>

// �X�N���[���V���b�g���烉���L���O�\�𒊏o����
cv::Mat extractScoreTable ( const cv::Mat ss );

// �����L���O�\����e�L�����N�^�X�R�A�ɐ؂蕪����
void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows );

cv::Mat removeNoise ( const cv::Mat image );

void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows );

cv::Mat scoreTable2Binary( const cv::Mat scoreTable );