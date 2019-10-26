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

cv::Mat withRayleighNoise(const cv::Mat& in, double sigmaSq) {
  cv::Mat out = in;

  std::random_device rd;
  std::mt19937 gen(rd());
  RayleighDistribution dist(sigmaSq);

  out.forEach<uchar>([&dist,&gen](uchar& value, const int pos[]) -> void {
    uchar noise = static_cast<uchar>(dist(gen));
    value = std::clamp(noise + value, 0, 255);
  });
  std::cout << std::endl;

  return out;
}
