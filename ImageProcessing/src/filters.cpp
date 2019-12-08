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


cv::Mat MedianFilter(const cv::Mat& mat, int radius)
{
    cv::Mat res = mat.clone();

    for (int i = 0; i < mat.rows; i++)
        for (int j = 0; j < mat.cols; j++)
        {
            std::vector<uchar> windows_pixels;
            uchar median;

            for (int k = -radius; k <= radius; k++)
                for (int l = -radius; l <= radius; l++)
                {
                    windows_pixels.push_back(mat.at<uchar>(std::clamp(i + k, 0, mat.rows - 1), std::clamp(j + l, 0, mat.cols - 1)));
                }

            std::sort(windows_pixels.begin(), windows_pixels.end());
            if (windows_pixels.size() % 2 == 0)
            {
                median = (windows_pixels[windows_pixels.size() / 2 - 1] + windows_pixels[windows_pixels.size() / 2]) / 2;
            }
            else
            {
                median = windows_pixels[windows_pixels.size() / 2];
            }

            res.at<uchar>(i, j) = median;
        }

    return res;
}


cv::Mat LowPassForDCTBlockSize(const cv::Mat& cosine, int T, int low_pass_T) {
  cv::Mat res = cv::Mat::zeros(cosine.size(), cosine.type());

  for (int i = 0; i < cosine.cols; i += T)
  {
      for (int j = 0; j < cosine.rows; j += T)
      {
          cosine(cv::Rect(i, j, low_pass_T, low_pass_T))
            .copyTo(res(cv::Rect(i, j, low_pass_T, low_pass_T)));
      }
  }

  return res;
}

cv::Mat ThresholdFilterForDCT(const cv::Mat& cosine, double threshold) {
  cv::Mat res = cosine.clone();
  res.setTo(0.0, cv::abs(cosine) < threshold);
  return res;
}
