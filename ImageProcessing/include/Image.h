#pragma once

#include <opencv2/opencv.hpp>
#include <highgui.h>


enum ImageType
{
    BGR,
    GRAY,
    BINARY
};

enum BGRChannel
{
    B = 0,
    G = 1,
    R = 2
};



class Image
{
public:

    virtual int open_file(std::string path) = 0;
    virtual int show();


    virtual int rows();
    virtual int cols();
    virtual void set_name(std::string image_name);
    virtual std::string get_name();

    virtual uchar & at(int x, int y) = 0;

protected:
    Image(ImageType type);
    Image(int rows, int cols, std::string name, ImageType type, int cv_type);

    cv::Mat image;
    std::string name;
    ImageType type;
};


class BGRImage : public Image
{
public:

    BGRImage();
    BGRImage(int rows, int cols, std::string name);

    int open_file(std::string path);

    void set_channel(BGRChannel channel);
    uchar & at(int x, int y);
    uchar & at(int x, int y, BGRChannel channel);
    BGRImage get_channel_image(BGRChannel channel);

protected:
    BGRChannel curr_channel;
};


class GrayImage : public Image
{
public:

    GrayImage();
    GrayImage(int rows, int cols, std::string name);

    int open_file(std::string path);

    uchar & at(int x, int y);

protected:
};
