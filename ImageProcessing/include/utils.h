#ifndef UTILS_H
#define UTILS_H
#include <opencv2/opencv.hpp>

double getMSSIM( const cv::Mat& i1, const cv::Mat& i2);

cv::Mat AlignImage(const cv::Mat& mat, int align_to);

cv::Mat DiscardAlignment(const cv::Mat& mat, int rows, int cols);

#endif  // UTILS_H
