#include "Image.h"


//===============================
//          Image
//===============================

Image::Image(ImageType type) :
    image(),
    name("no name"),
    type(type)
{}

Image::Image(int rows, int cols, std::string name, ImageType type, int cv_type) :
    image(rows, cols, cv_type, cv::Scalar(0, 0, 0)),
    name(name),
    type(type)
{}

int Image::show()
{
    if (!image.data)
        return -1;

    cv::namedWindow(name, cv::WINDOW_AUTOSIZE);
    imshow(name, image);

    return 0;
}

void Image::set_name(std::string image_name)
{
    name = image_name;
}

std::string Image::get_name()
{
    return name;
}

int Image::rows()
{
    return image.rows;
}

int Image::cols()
{
    return image.cols;
}


//===============================
//          BGRImage
//===============================

BGRImage::BGRImage() :
    Image(BGR),
    curr_channel(B)
{}

BGRImage::BGRImage(int rows, int cols, std::string name) :
    Image(rows, cols, name, BGR, CV_8UC3),
    curr_channel(B)
{}

int BGRImage::open_file(std::string path)
{
    image = cv::imread(path, CV_LOAD_IMAGE_COLOR);

    if (!image.data)
        return -1;

    // save the opened file's name
    name = path.substr(path.find_last_of("/\\") + 1);
    name = name.substr(0, name.find_last_of('.'));
    
    type = BGR;
    return 0;
}

void BGRImage::set_channel(BGRChannel channel)
{
    curr_channel = channel;
}

uchar & BGRImage::at(int x, int y)
{
    return image.at<cv::Vec3b>(x, y)[curr_channel];
}

uchar & BGRImage::at(int x, int y, BGRChannel channel)
{
    return image.at<cv::Vec3b>(x, y)[channel];
}

BGRImage BGRImage::get_channel_image(BGRChannel channel)
{
    std::string channel_name(name);
    switch (channel)
    {
    case R: channel_name.append(" | Red"); break;
    case G: channel_name.append(" | Green"); break;
    case B: channel_name.append(" | Blue"); break;
    }

    BGRImage channel_image(image.rows, image.cols, channel_name);

    channel_image.set_channel(channel);
    for (int i = 0; i < this->rows(); i++)
        for (int j = 0; j < this->cols(); j++)
            channel_image.at(i, j) = this->at(i, j, channel);

    return channel_image;
}


//===============================
//          GrayImage
//===============================

GrayImage::GrayImage() :
    Image(GRAY)
{}

GrayImage::GrayImage(int rows, int cols, std::string name) :
    Image(rows, cols, name, GRAY, CV_8UC1)
{}

int GrayImage::open_file(std::string path)
{
    image = cv::imread(path, CV_LOAD_IMAGE_GRAYSCALE);

    if (!image.data)
        return -1;

    // save the opened file's name
    name = path.substr(path.find_last_of("/\\") + 1);
    name = name.substr(0, name.find_last_of('.'));

    type = GRAY;
    return 0;
}

uchar & GrayImage::at(int x, int y)
{
    return image.at<uchar>(x, y);
}