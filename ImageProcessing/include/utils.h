#ifndef UTILS_H
#define UTILS_H
#include <opencv2/opencv.hpp>

double getMSSIM( const cv::Mat& i1, const cv::Mat& i2);

cv::Mat AlignImage(const cv::Mat& mat, int align_to);

cv::Mat DiscardAlignment(const cv::Mat& mat, int rows, int cols);

void WriteFile(const cv::Mat& mat, std::string name, int elem_size);

void ReadFile(cv::Mat& mat, std::string name, int elem_size);

#endif  // UTILS_H
