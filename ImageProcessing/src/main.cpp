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
    cv::imwrite("Rayleigh_Noise.png", rayleigh);
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
    cv::imshow("rayleigh Opening", opening);
    cv::imwrite("rayleigh_Opening.png", opening);

    cv::Mat rayleigh_median;
    rayleigh_median = MedianFilter(rayleigh, 1);
    cv::imshow("Rayleigh Median Filter", rayleigh_median);
    cv::imwrite("Rayleigh Median Filter.png", rayleigh_median);

    cv::Mat rayleigh_blured;
    cv::GaussianBlur(rayleigh, rayleigh_blured, cv::Size(3, 3), 2.5);
    cv::imshow("Gaussian Blur for Rayleigh Noise", rayleigh_blured);
    cv::imwrite("Gaussian Blur for Rayleigh Noise.png", rayleigh_blured);


    cv::Mat gamma;
    gamma = GammaNoise(image, 2.0, 5.0);
    cv::imshow("Gamma Noise", gamma);
    cv::imwrite("Gamma Noise.png", gamma);

    cv::Mat median;
    median = MedianFilter(gamma, 1);
    cv::imshow("Median Filter", median);
    cv::imwrite("Median Filter.png", median);

    cv::Mat gamma_opening = morphOpening(gamma, kernel);
    cv::imshow("Gamma Opening", gamma_opening);
    cv::imwrite("Gamma_Opening.png", gamma_opening);

    cv::Mat gamma_blured;
    cv::GaussianBlur(gamma, gamma_blured, cv::Size(3, 3), 2.5);
    cv::imshow("Gaussian Blur for Gamma Noise", gamma_blured);
    cv::imwrite("Gaussian Blur for Gamma Noise.png", gamma_blured);

    std::cout << "SSIM(Rayleigh noise, gray) = " << getMSSIM(rayleigh, image) << std::endl;
    std::cout << "SSIM(Rayleigh opening, gray) = " << getMSSIM(opening, image) << std::endl;
    std::cout << "SSIM(Gamma opening, gray) = " << getMSSIM(gamma_opening, image) << std::endl;
    std::cout << "SSIM(Gaussian blur (Rayleigh noise), gray) = " << getMSSIM(rayleigh_blured, image) << std::endl;
    std::cout << "SSIM(Gamma noise, gray) = " << getMSSIM(gamma, image) << std::endl;
    std::cout << "SSIM(Gamma Median filter, gray) = " << getMSSIM(median, image) << std::endl;
    std::cout << "SSIM(Rayleigh Median filter, gray) = " << getMSSIM(rayleigh_median, image) << std::endl;
    std::cout << "SSIM(Gaussian blur (Gamma noise), gray) = " << getMSSIM(gamma_blured, image) << std::endl;
}


void lab3(const cv::Mat& image)
{
    cv::Mat image_64f;
    image.convertTo(image_64f, CV_64F);
    image_64f /= 255;

    std::vector<int> rows;
    std::vector<int> cols;

    cv::Mat wavelet = Wavelet(image_64f, 2, rows, cols);
    cv::imshow("Wavelet transform", wavelet);
    cv::imwrite("Wavelet transform.png", wavelet * 255);

    double min, max;
    cv::minMaxLoc(wavelet, &min, &max);
    cv::Mat wavelet_shifted = (wavelet - min) / (max - min);
    cv::imshow("Wavelet transform shifted", wavelet_shifted);
    cv::imwrite("Wavelet transform shifted.png", wavelet_shifted * 255);

    cv::Mat wavelet_reverse = WaveletReverse(wavelet, rows, cols);
    cv::imshow("Reverse wavelet transform", wavelet_reverse);

    std::cout << "SSIM(Reverse wavelet, gray) = " << getMSSIM(wavelet_reverse, image_64f) << std::endl;


    int block_size = 8;
    cv::Mat cosine = Cosine(AlignImage(image_64f, block_size), block_size);

    cv::imshow("Cosine transform", cosine);
    cv::imwrite("Cosine transform.png", cosine * 255);

    cv::minMaxLoc(cosine, &min, &max);
    cv::Mat cosine_shifted = (cosine - min) / (max - min);
    cv::imshow("Cosine transform shifted", cosine_shifted);
    cv::imwrite("Cosine transform shifted.png", cosine_shifted * 255);

    cv::Mat cosine_reverse = DiscardAlignment(CosineReverse(cosine, block_size), image_64f.rows, image_64f.cols);
    cv::imshow("Reverse cosine transform", cosine_reverse);

    std::cout << "SSIM(Reverse cosine, gray) = " << getMSSIM(cosine_reverse, image_64f) << std::endl;
}


void lab3_denoise(const cv::Mat& image)
{
    cv::Mat gamma;
    gamma = GammaNoise(image, 2.0, 5.0);

    std::cout << "SSIM(Gamma, gray) = " << getMSSIM(gamma, image) << std::endl;
    cv::imwrite("gamma.png", gamma);

    cv::Mat image_64f_gamma;
    gamma.convertTo(image_64f_gamma, CV_64F);
    image_64f_gamma /= 255;

    cv::imshow("Gamma Noise", image_64f_gamma);

    //==============   DCT  =======================
    int block_size = 8;
    cv::Mat cosine = Cosine(AlignImage(image_64f_gamma, block_size), block_size);

    cosine = ThresholdFilterForDCT(cosine, 0.1);

    cv::Mat cosine_reverse = DiscardAlignment(CosineReverse(cosine, block_size), image_64f_gamma.rows, image_64f_gamma.cols);
    cv::imshow("Reverse cosine transform", cosine_reverse);

    cosine_reverse.convertTo(cosine_reverse, CV_8UC1, 255);
    std::cout << "SSIM(Reverse cosine, gray) = " << getMSSIM(cosine_reverse, image) << std::endl;
    cv::imwrite("Cosine denoise.png", cosine_reverse);


    //==============  Wavelet  ====================
    std::vector<int> rows;
    std::vector<int> cols;
    cv::Mat wavelet = Wavelet(image_64f_gamma, 0, rows, cols);
    
    wavelet = WaveletDenoise(wavelet, 3, 0.1);

    cv::Mat wavelet_reverse = WaveletReverse(wavelet, rows, cols);
    cv::imshow("Reverse wavelet transform", wavelet_reverse);

    wavelet_reverse.convertTo(wavelet_reverse, CV_8UC1, 255);
    std::cout << "SSIM(Reverse wavelet, gray) = " << getMSSIM(wavelet_reverse, image) << std::endl;
    cv::imwrite("Wavelet denoise.png", wavelet_reverse);
}


void lab3_encode(const cv::Mat& image)
{
    cv::Mat image_64f;
    image.convertTo(image_64f, CV_64F);
    image_64f /= 255;


    //==============   DCT  =======================
    int block_size = 8;
    int qp_cosine = 40;
    std::string cosine_file_name = "cosine_qp" + std::to_string(qp_cosine) + "_bs" + std::to_string(block_size);

    cv::Mat cosine = Cosine(AlignImage(image_64f, block_size), block_size);
    cv::imshow("Cosine transform", cosine);

    cv::Mat cosine_int;
    cosine.convertTo(cosine_int, CV_16SC1, qp_cosine);
    WriteFile(cosine_int, cosine_file_name, 2);

    ReadFile(cosine_int, cosine_file_name, 2);
    cosine_int.convertTo(cosine, CV_64F);
    cosine /= qp_cosine;

    cv::Mat cosine_reverse = DiscardAlignment(CosineReverse(cosine, block_size), image_64f.rows, image_64f.cols);
    cv::imshow("Reverse cosine transform", cosine_reverse);

    cosine_reverse.convertTo(cosine_reverse, CV_8UC1, 255);
    std::cout << "qp = " << qp_cosine << std::endl;
    std::cout << "SSIM(Reverse cosine, gray) = " << getMSSIM(cosine_reverse, image) << std::endl;
    cv::imwrite("cosine_reverse " + std::to_string(qp_cosine) + ".png", cosine_reverse);

    //==============  Wavelet  ====================
    std::vector<int> rows;
    std::vector<int> cols;
    int qp_wavelet = 150;
    std::string wavelet_file_name = "wavelet_qp" + std::to_string(qp_wavelet);

    cv::Mat wavelet = Wavelet(image_64f, 0, rows, cols);
    cv::imshow("Wavelet transform", wavelet);

    cv::Mat wavelet_int;
    wavelet.convertTo(wavelet_int, CV_16SC1, qp_wavelet);
    WriteFile(wavelet_int, wavelet_file_name, 2);

    ReadFile(wavelet_int, wavelet_file_name, 2);
    wavelet_int.convertTo(wavelet, CV_64F);
    wavelet /= qp_wavelet;

    cv::Mat wavelet_reverse = WaveletReverse(wavelet, rows, cols);
    cv::imshow("Reverse wavelet transform", wavelet_reverse);

    wavelet_reverse.convertTo(wavelet_reverse, CV_8UC1, 255);
    std::cout << "SSIM(Reverse wavelet, gray) = " << getMSSIM(wavelet_reverse, image) << std::endl;
    cv::imwrite("wavelet_reverse.png", wavelet_reverse);
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
  cv::imwrite("Gray.png", gray);


  //lab3(gray);

  //lab3_denoise(gray);

  lab3_encode(gray);

  //lab1(gray);

  cv::waitKey();

}
