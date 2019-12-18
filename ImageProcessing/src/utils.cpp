#include <fstream>
#include "utils.h"

using namespace cv;

// from opencv.org
double getMSSIM( const Mat& i1, const Mat& i2)
{
 const double C1 = 6.5025, C2 = 58.5225;
 /***************************** INITS **********************************/
 int d     = CV_32F;

 Mat I1, I2;
 i1.convertTo(I1, d);           // cannot calculate on one byte large values
 i2.convertTo(I2, d);

 Mat I2_2   = I2.mul(I2);        // I2^2
 Mat I1_2   = I1.mul(I1);        // I1^2
 Mat I1_I2  = I1.mul(I2);        // I1 * I2

 /***********************PRELIMINARY COMPUTING ******************************/

 Mat mu1, mu2;   //
 GaussianBlur(I1, mu1, Size(11, 11), 1.5);
 GaussianBlur(I2, mu2, Size(11, 11), 1.5);

 Mat mu1_2   =   mu1.mul(mu1);
 Mat mu2_2   =   mu2.mul(mu2);
 Mat mu1_mu2 =   mu1.mul(mu2);

 Mat sigma1_2, sigma2_2, sigma12;

 GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
 sigma1_2 -= mu1_2;

 GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
 sigma2_2 -= mu2_2;

 GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
 sigma12 -= mu1_mu2;

 ///////////////////////////////// FORMULA ////////////////////////////////
 Mat t1, t2, t3;

 t1 = 2 * mu1_mu2 + C1;
 t2 = 2 * sigma12 + C2;
 t3 = t1.mul(t2);              // t3 = ((2*mu1_mu2 + C1).*(2*sigma12 + C2))

 t1 = mu1_2 + mu2_2 + C1;
 t2 = sigma1_2 + sigma2_2 + C2;
 t1 = t1.mul(t2);               // t1 =((mu1_2 + mu2_2 + C1).*(sigma1_2 + sigma2_2 + C2))

 Mat ssim_map;
 divide(t3, t1, ssim_map);      // ssim_map =  t3./t1;

 return  mean( ssim_map )[0]; // mssim = average of ssim map
}



cv::Mat AlignImage(const cv::Mat& mat, int align_to)
{
    int rows_new = mat.rows, cols_new = mat.cols;

    if (mat.rows % align_to != 0)
        rows_new = mat.rows + align_to - mat.rows % align_to;
    if (mat.cols % align_to != 0)
        cols_new = mat.cols + align_to - mat.cols % align_to;

    cv::Mat res(rows_new, cols_new, mat.type(), Scalar(0));

    mat.copyTo(res(cv::Rect(0, 0, mat.cols, mat.rows)));

    return res;
}

cv::Mat DiscardAlignment(const cv::Mat& mat, int rows, int cols)
{
    return mat(cv::Rect(0, 0, cols, rows));
}


// elem_size - size of mat element in bytes
void WriteFile(const cv::Mat& mat, std::string name, int elem_size)
{
    std::ofstream fout(name, std::ios::out | std::ios::binary | std::ios::trunc);
    fout.write((char*)(mat.data), mat.rows * mat.cols * elem_size);
    fout.close();
}

void ReadFile(cv::Mat& mat, std::string name, int elem_size)
{
    std::ifstream fin(name, std::ios::in | std::ios::binary);
    fin.read((char*)(mat.data), mat.rows * mat.cols * elem_size);
    fin.close();
}
