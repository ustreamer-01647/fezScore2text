#pragma once

// スクリーンショットからランキング表を抽出する
cv::Mat extractScoreTable ( const cv::Mat ss );

// ランキング表から各キャラクタスコアに切り分ける
void extractScoreRows( const cv::Mat scoreTable, cv::vector<cv::Mat> &scoreRows );
