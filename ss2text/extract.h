#pragma once

#include <opencv2/core/core.hpp>

// スクリーンショットからランキング表を抽出する
cv::Mat extractScoreTable ( const cv::Mat ss );

// ランキング表から各キャラクタスコアに切り分ける
void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows );

cv::Mat removeNoise ( const cv::Mat image );

void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows );

cv::Mat scoreTable2Binary( const cv::Mat scoreTable );