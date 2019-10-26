#ifndef NOISE_H
#define NOISE_H
#include <opencv2/opencv.hpp>

cv::Mat withRayleighNoise(const cv::Mat& in, double sigmaSq);

#endif  // NOISE_H
