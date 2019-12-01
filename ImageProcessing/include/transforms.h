#ifndef TRANSFORMS_H
#define TRANSFORMS_H

#include <opencv2/opencv.hpp>

cv::Mat Wavelet(const cv::Mat& mat, int k, std::vector<int>& rows_arr, std::vector<int>& cols_arr);

cv::Mat WaveletReverse(const cv::Mat& mat, const std::vector<int>& rows_arr, const std::vector<int>& cols_arr);

cv::Mat Cosine(const cv::Mat& mat, int T);

cv::Mat CosineReverse(const cv::Mat& mat, int T);

#endif  // TRANSFORMS_H
