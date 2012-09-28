#pragma once

int recognizeDigit ( const cv::Mat image );
int recognizeInteger ( const cv::Mat image );
void recognize ( const cv::vector<cv::Mat> images, std::vector<struct Score> &scores );
void recognize ( const cv::Mat scoreTable, const cv::Mat scoreTableBinary, std::vector<struct Score> &scores );
