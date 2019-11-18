#include <random>
#include "noise.h"

class RayleighDistribution {
   std::uniform_real_distribution<> uniform;
   double sigmaSq;
public:
   explicit RayleighDistribution(double sigmaSq): uniform(0.0, 1.0), sigmaSq(sigmaSq) {}

   template<typename Generator>
   double operator()(Generator& g);
};

template<typename Generator>
double RayleighDistribution::operator()(Generator& g) {
  double cdf = uniform(g);
  return std::sqrt(-std::log(1 - cdf) / (2*sigmaSq));
}

cv::Mat withRayleighNoise(const cv::Mat& in, double sigmaSq, uint noiseHist[256]) {
  cv::Mat out = in.clone();

  std::random_device rd;
  std::mt19937 gen(rd());
  RayleighDistribution dist(sigmaSq);

  std::fill(noiseHist, noiseHist + 256, 0U);

  out.forEach<uchar>([&dist, &gen, noiseHist](uchar& value, const int pos[])
  -> void {
    uchar noise = static_cast<uchar>(dist(gen));
    noiseHist[noise]++;
    value = std::clamp(noise + value, 0, 255);
  });
  std::cout << std::endl;

  for (size_t i = 0; i < 256; ++i) {
    noiseHist[i] = noiseHist[i];
  }

  return out;
}



cv::Mat GammaNoise(const cv::Mat& mat, double a, double b)
{
    cv::Mat res = mat.clone();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::gamma_distribution<> dist(a, b);

    uint noiseHist[256];
    std::fill(noiseHist, noiseHist + 256, 0U);

    for (int i = 0; i < res.rows; i++)
        for (int j = 0; j < res.cols; j++)
        {
            uchar noise = static_cast<uchar>(dist(gen));
            noiseHist[noise]++;
            res.at<uchar>(i, j) = std::clamp(noise + res.at<uchar>(i, j), 0, 255);
        }

    return res;
}
