#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include <stdlib.h>
#include <iostream>

#include "noise.h"
#include "filters.h"
#include "utils.h"



void rayleigh_opening(cv::Mat image)
{
    cv::Mat gray;
    cv::cvtColor(image, gray, CV_BGR2GRAY);
    cv::imshow("3. Gray (press any key to continue)", gray);
    // cv::waitKey();


    uint noiseHist[256];
    cv::Mat noisy = withRayleighNoise(gray, 0.004, noiseHist);
    cv::imshow("3. Noisy (Rayleigh) (press any key to continue)", noisy);
    // cv::waitKey();

    for (size_t i = 0; i < 256; ++i) {
        for (size_t j = 0; j < noiseHist[i] >> 7; ++j) {
            std::cout << '*';
        }
        std::cout << std::endl;

    }

    uchar v = 10U;
    const cv::Mat kernel = (cv::Mat_<uchar>(5, 5) <<
        v, v, v, v, v,
        v, v, v, v, v,
        v, v, v, v, v,
        v, v, v, v, v,
        v, v, v, v, v);

    cv::Mat filtered = morphOpening(noisy, kernel);
    cv::imshow("4. Morphological Opening (press any key to continue)", filtered);

    cv::Mat blured;
    cv::GaussianBlur(gray, blured, cv::Size(3, 3), 2.5);
    cv::imshow("5. Gaussian Blur (press any key to continue)", blured);

    std::cout << "SSIM(filtered, gray) = " << getMSSIM(filtered, gray) << std::endl;
    std::cout << "SSIM(blured, gray) = " << getMSSIM(blured, gray) << std::endl;
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

  cv::imshow("1. Original (press any key to continue)", image);
  // cv::waitKey();


  rayleigh_opening(image);


  cv::waitKey();

}
