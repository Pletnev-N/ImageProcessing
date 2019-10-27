#ifndef FILTERS_H
#define FILTERS_H
#include <opencv2/opencv.hpp>

cv::Mat morphOpening(const cv::Mat& in, const cv::Mat& kernel);

#endif  // FILTERS_H
