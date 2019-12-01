#include "transforms.h"


void WaveletRow(cv::Mat& mat, int row, int cols)
{
    double* tmp = new double[cols];

    for (int i = 1, data_i = 0, coef_i = (cols / 2) + (cols % 2); i < cols; i+=2, data_i++, coef_i++)
    {
        double avg = (mat.at<double>(row, i - 1) + mat.at<double>(row, i)) / 2;
        tmp[coef_i] = mat.at<double>(row, i - 1) - avg;
        //tmp[coef_i] *= 3;
        tmp[data_i] = avg;
    }

    if (cols % 2 == 1)
        tmp[(cols / 2) + (cols % 2) - 1] = mat.at<double>(row, cols - 1);

    for (int i = 0; i < cols; i++)
    {
        mat.at<double>(row, i) = tmp[i];
    }

    delete[] tmp;
}

void WaveletCol(cv::Mat& mat, int col, int rows)
{
    double* tmp = new double[rows];

    for (int i = 1, data_i = 0, coef_i = (rows / 2) + (rows % 2); i < rows; i += 2, data_i++, coef_i++)
    {
        double avg = (mat.at<double>(i - 1, col) + mat.at<double>(i, col)) / 2;
        tmp[coef_i] = mat.at<double>(i - 1, col) - avg;
        //tmp[coef_i] *= 3;
        tmp[data_i] = avg;
    }

    if (rows % 2 == 1)
        tmp[(rows / 2) + (rows % 2) - 1] = mat.at<double>(rows - 1, col);

    for (int i = 0; i < rows; i++)
    {
        mat.at<double>(i, col) = tmp[i];
    }

    delete[] tmp;
}

cv::Mat Wavelet(const cv::Mat& mat, int k, std::vector<int>& rows_arr, std::vector<int>& cols_arr)
{
    cv::Mat res = mat.clone();

    int rows = res.rows;
    int cols = res.cols;
    rows_arr.push_back(rows);
    cols_arr.push_back(cols);

    bool stop = false;
    int i = 0;

    while (!stop)
    {
        for (int j = 0; j < rows; j++)
        {
            WaveletRow(res, j, cols);
        }

        for (int j = 0; j < cols; j++)
        {
            WaveletCol(res, j, rows);
        }

        rows = (rows / 2) + (rows % 2);
        cols = (cols / 2) + (cols % 2);
        rows_arr.push_back(rows);
        cols_arr.push_back(cols);
        i++;

        if ((i == k) || (rows == 1) || (cols == 1))
            stop = true;
    }

    return res;
}


void WaveletReverseRow(cv::Mat& mat, int row, int cols)
{
    double* tmp = new double[cols];

    for (int i = 1, data_i = 0, coef_i = (cols / 2) + (cols % 2); i < cols; i += 2, data_i++, coef_i++)
    {
        tmp[i - 1] = mat.at<double>(row, data_i) + mat.at<double>(row, coef_i);
        tmp[i] = mat.at<double>(row, data_i) - mat.at<double>(row, coef_i);
    }

    if (cols % 2 == 1)
        tmp[cols - 1] = mat.at<double>(row, (cols / 2) + (cols % 2) - 1);

    for (int i = 0; i < cols; i++)
    {
        mat.at<double>(row, i) = tmp[i];
    }

    delete[] tmp;
}

void WaveletReverseCol(cv::Mat& mat, int col, int rows)
{
    double* tmp = new double[rows];

    for (int i = 1, data_i = 0, coef_i = (rows / 2) + (rows % 2); i < rows; i += 2, data_i++, coef_i++)
    {
        tmp[i - 1] = mat.at<double>(data_i, col) + mat.at<double>(coef_i, col);
        tmp[i] = mat.at<double>(data_i, col) - mat.at<double>(coef_i, col);
    }

    if (rows % 2 == 1)
        tmp[rows - 1] = mat.at<double>((rows / 2) + (rows % 2) - 1, col);

    for (int i = 0; i < rows; i++)
    {
        mat.at<double>(i, col) = tmp[i];
    }

    delete[] tmp;
}

cv::Mat WaveletReverse(const cv::Mat& mat, const std::vector<int>& rows_arr, const std::vector<int>& cols_arr)
{
    cv::Mat res = mat.clone();

    for (int i = rows_arr.size() - 2; i >= 0; i--)
    {
        for (int j = 0; j < cols_arr[i]; j++)
        {
            WaveletReverseCol(res, j, rows_arr[i]);
        }

        for (int j = 0; j < rows_arr[i]; j++)
        {
            WaveletReverseRow(res, j, cols_arr[i]);
        }
    }

    return res;
}

