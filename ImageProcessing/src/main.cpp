#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include <stdlib.h>
#include <iostream>

#include "noise.h"
#include "filters.h"
#include "utils.h"
#include "transforms.h"



void lab1(const cv::Mat& image)
{
    uint noiseHist[256];
    cv::Mat rayleigh = withRayleighNoise(image, 0.004, noiseHist);
    cv::imshow("Rayleigh Noise", rayleigh);
    // cv::waitKey();

    //for (size_t i = 0; i < 256; ++i) {
    //    for (size_t j = 0; j < noiseHist[i] >> 7; ++j) {
    //        std::cout << '*';
    //    }
    //    std::cout << std::endl;

    //}

    uchar v = 10U;
    const cv::Mat kernel = (cv::Mat_<uchar>(3, 3) <<
        v, v, v,
        v, v, v,
        v, v, v);

    cv::Mat opening = morphOpening(rayleigh, kernel);
    cv::imshow("Morphological Opening (press any key to continue)", opening);

    cv::Mat rayleigh_blured;
    cv::GaussianBlur(rayleigh, rayleigh_blured, cv::Size(3, 3), 2.5);
    cv::imshow("Gaussian Blur for Rayleigh Noise", rayleigh_blured);


    cv::Mat gamma;
    gamma = GammaNoise(image, 2.0, 5.0);
    cv::imshow("Gamma Noise", gamma);

    cv::Mat median;
    median = MedianFilter(gamma, 1);
    cv::imshow("Median Filter", median);

    cv::Mat gamma_blured;
    cv::GaussianBlur(gamma, gamma_blured, cv::Size(3, 3), 2.5);
    cv::imshow("Gaussian Blur for Gamma Noise", gamma_blured);


    std::cout << "SSIM(Rayleigh noise, gray) = " << getMSSIM(rayleigh, image) << std::endl;
    std::cout << "SSIM(Morphological opening, gray) = " << getMSSIM(opening, image) << std::endl;
    std::cout << "SSIM(Gaussian blur (Rayleigh noise), gray) = " << getMSSIM(rayleigh_blured, image) << std::endl;
    std::cout << "SSIM(Gamma noise, gray) = " << getMSSIM(gamma, image) << std::endl;
    std::cout << "SSIM(Median filter, gray) = " << getMSSIM(median, image) << std::endl;
    std::cout << "SSIM(Gaussian blur (Gamma noise), gray) = " << getMSSIM(gamma_blured, image) << std::endl;
}


void lab3(const cv::Mat& image)
{    
    cv::Mat image_64f;
    image.convertTo(image_64f, CV_64F);
    image_64f /= 255;

    std::vector<int> rows;
    std::vector<int> cols;

    cv::Mat wavelet = Wavelet(image_64f, 0, rows, cols);
    cv::imshow("Wavelet transform", wavelet);

    cv::Mat wavelet_reverse = WaveletReverse(wavelet, rows, cols);
    cv::imshow("Reverse wavelet transform", wavelet_reverse);

    std::cout << "SSIM(Reverse wavelet, gray) = " << getMSSIM(wavelet_reverse, image_64f) << std::endl;
}


//arg 1 - source image file
int main(int argc, char **argv)
{
  cv::Mat image;

  if (argc != 2) {
    std::cerr << "Please provide input file name." << std::endl;
    return 1;
  }

  // 1:
  image = cv::imread(argv[1]);

  if (image.empty()) {
    std::cerr << "Could not read the image." << std::endl;
    return 2;
  }

  //cv::imshow("Original (press any key to continue)", image);

  cv::Mat gray;
  cv::cvtColor(image, gray, CV_BGR2GRAY);
  cv::imshow("Gray (press any key to continue)", gray);


  lab3(gray);

  //lab1(gray);

  cv::waitKey();

}
