#pragma once

// �X�N���[���V���b�g���烉���L���O�\�𒊏o����
cv::Mat extractScoreTable ( const cv::Mat ss );

// �����L���O�\����e�L�����N�^�X�R�A�ɐ؂蕪����
void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows );
