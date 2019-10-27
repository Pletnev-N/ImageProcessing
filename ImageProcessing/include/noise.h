#ifndef NOISE_H
#define NOISE_H
#include <opencv2/opencv.hpp>

cv::Mat withRayleighNoise(const cv::Mat& in, double sigmaSq, uint noiseHist[256]);

#endif  // NOISE_H
