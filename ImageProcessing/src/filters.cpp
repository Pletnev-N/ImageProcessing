#include "filters.h"
#include <algorithm>

cv::Mat dilation(const cv::Mat& in, const cv::Mat& kernel) {
  cv::Size ksize = kernel.size();
  cv::Mat out;
  out.create(in.size(), in.type());

  out.forEach<uchar>([&in, &kernel, ksize]
    (uchar& value, const int position[]) -> void {
      if (position[0] < ksize.width || position[1] < ksize.height) {
        return;
      }
      int max = 0;
      for (int i = 0; i < ksize.width; ++i) {
        for (int j = 0; j < ksize.height; ++j) {
          max = std::max(max,
            in.at<uchar>(position[0] - i, position[1] - j)
              + kernel.at<uchar>(i, j));
        }
      }
      value = static_cast<uchar>(std::clamp(max, 0, 255));
  });

  return out;
}

cv::Mat erosion(const cv::Mat& in, const cv::Mat& kernel) {
  cv::Size ksize = kernel.size();
  cv::Mat out;
  out.create(in.size(), in.type());

  out.forEach<uchar>([&in, &kernel, ksize]
    (uchar& value, const int position[]) -> void {
      if (position[0] < ksize.width || position[1] < ksize.height) {
        return;
      }
      int min = 255;
      for (int i = 0; i < ksize.width; ++i) {
        for (int j = 0; j < ksize.height; ++j) {
          min = std::min(min,
            in.at<uchar>(position[0] - i, position[1] - j)
              - kernel.at<uchar>(i, j));
        }
      }
      value = static_cast<uchar>(std::clamp(min, 0, 255));
  });

  return out;
}

cv::Mat morphOpening(const cv::Mat& in, const cv::Mat& kernel) {
  cv::Mat t = erosion(in, kernel);
  return dilation(t, kernel);
}
