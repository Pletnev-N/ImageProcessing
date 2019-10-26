#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <stack>
#include <iterator>
#include "noise.h"

#if defined(_WIN32)
#include <conio.h>
#endif

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
  cv::waitKey();

  cv::Mat gray;
  cv::cvtColor(image, gray, CV_BGR2GRAY);
  cv::imshow("3. Gray (press any key to continue)", gray);
  cv::waitKey();

  cv::Mat noisy = withRayleighNoise(gray, 0.0004);
  cv::imshow("3. Noisy (Rayleigh) (press any key to continue)", noisy);
  cv::waitKey();

}
