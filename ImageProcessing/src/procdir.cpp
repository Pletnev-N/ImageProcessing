#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include <stdlib.h>
#include <iostream>
#include <filesystem>
#include <string>
#include <numeric>

#include "noise.h"
#include "filters.h"
#include "utils.h"

using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

struct ssim_results {
  double rayleigh;
  double rayleigh_opening;
  double rayleigh_blured;
  double gamma;
  double gamma_median;
  double gamma_blured;
  double rayleigh_median;
  double gamma_opening;
};

ssim_results operator+(const ssim_results& a, const ssim_results b) {
  return ssim_results{
    a.rayleigh + b.rayleigh,
    a.rayleigh_opening + b.rayleigh_opening,
    a.rayleigh_blured + b.rayleigh_blured,
    a.gamma + b.gamma,
    a.gamma_median + b.gamma_median,
    a.gamma_blured + b.gamma_blured,
    a.rayleigh_median + b.rayleigh_median,
    a.gamma_opening + b.gamma_opening
  };
}

ssim_results operator/(const ssim_results& a, double i) {
  return ssim_results{
    a.rayleigh  /  i,
    a.rayleigh_opening  /  i,
    a.rayleigh_blured  /  i,
    a.gamma  /  i,
    a.gamma_median  /  i,
    a.gamma_blured  /  i,
    a.rayleigh_median  /  i,
    a.gamma_opening  /  i
  };
}

std::ostream& operator<<(std::ostream& out, const ssim_results r) {
  out << "rayleigh=" << r.rayleigh << std::endl;
  out << "rayleigh_opening=" << r.rayleigh_opening << std::endl;
  out << "rayleigh_blured=" << r.rayleigh_blured << std::endl;
  out << "gamma=" << r.gamma << std::endl;
  out << "gamma_median=" << r.gamma_median << std::endl;
  out << "gamma_blured=" << r.gamma_blured << std::endl;
  out << "rayleigh_median=" << r.rayleigh_median << std::endl;
  out << "gamma_opening=" << r.gamma_opening << std::endl;
  return out;
}

ssim_results process_single_image(const cv::Mat& image)
{
    uint noiseHist[256];
    cv::Mat rayleigh = withRayleighNoise(image, 0.004, noiseHist);

    uchar v = 10U;
    const cv::Mat kernel = (cv::Mat_<uchar>(3, 3) <<
        v, v, v,
        v, v, v,
        v, v, v);
    cv::Mat rayleigh_opening = morphOpening(rayleigh, kernel);

    cv::Mat rayleigh_blured;
    cv::GaussianBlur(rayleigh, rayleigh_blured, cv::Size(3, 3), 2.5);

    cv::Mat gamma = GammaNoise(image, 2.0, 5.0);

    cv::Mat gamma_median = MedianFilter(gamma, 1);

    cv::Mat gamma_blured;
    cv::GaussianBlur(gamma, gamma_blured, cv::Size(3, 3), 2.5);

    cv::Mat rayleigh_median = MedianFilter(rayleigh, 1);

    cv::Mat gamma_opening = morphOpening(gamma, kernel);

    return ssim_results{
      getMSSIM(rayleigh, image),
      getMSSIM(rayleigh_opening, image),
      getMSSIM(rayleigh_blured, image),
      getMSSIM(gamma, image),
      getMSSIM(gamma_median, image),
      getMSSIM(gamma_blured, image),
      getMSSIM(rayleigh_median, image),
      getMSSIM(gamma_opening, image),
    };
}

//arg 1 - source image file
int main(int argc, char **argv)
{
  if (argc != 2) {
    std::cerr << "Please provide input dir path." << std::endl;
    return 1;
  }

  const std::string png_ext{".png"};
  std::vector<ssim_results> results;
  cv::Mat gray;
  int i = 0;

  for (const auto& dirEntry : recursive_directory_iterator(argv[1])) {
    // if ends with .png process it
    if (dirEntry.path().extension() == png_ext) {
      std::string file_path = dirEntry.path().string();
      std::cout << (++i) << ". Processing " << file_path << std::endl;
      cv::Mat image = cv::imread(file_path);
      if (image.empty()) {
        std::cerr << "Could not read the image." << dirEntry << std::endl;
        continue;
      }
      cv::cvtColor(image, gray, CV_BGR2GRAY);
      results.push_back(process_single_image(gray));
    }
  }

  ssim_results average = std::accumulate(results.begin(), results.end(),
    ssim_results{}) / static_cast<double>(results.size());

  std::cout << average;
}
