#ifndef NOISE_H
#define NOISE_H
#include <opencv2/opencv.hpp>

cv::Mat withRayleighNoise(const cv::Mat& in, double sigmaSq, uint noiseHist[256]);

cv::Mat GammaNoise(const cv::Mat& mat, double a, double b);

#endif  // NOISE_H
