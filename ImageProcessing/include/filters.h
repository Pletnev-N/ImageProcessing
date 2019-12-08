#ifndef FILTERS_H
#define FILTERS_H
#include <opencv2/opencv.hpp>

cv::Mat morphOpening(const cv::Mat& in, const cv::Mat& kernel);

cv::Mat MedianFilter(const cv::Mat& mat, int radius);

cv::Mat LowPassForDCTBlockSize(const cv::Mat& cosine, int T, int low_pass_T);
cv::Mat ThresholdFilterForDCT(const cv::Mat& cosine, double threshold);

#endif  // FILTERS_H
