#include <opencv2/opencv.hpp>
#include <cxcore.h>
#include <highgui.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>
#include <stack>
#include <iterator>

#if defined(_WIN32)
#include <conio.h>
#endif


#include "Image.h"


//arg 1 - source image file
int main(int argc, char **argv)
{
    if (argc == 1)
    {
        return -1;
    }

    BGRImage image_orig;
    image_orig.open_file(argv[1]);

    GrayImage image_gray;
    image_gray.open_file(argv[1]);
    image_gray.set_name("Gray");

    BGRImage red_channel = image_orig.get_channel_image(R);

    image_orig.show();
    image_gray.show();
    red_channel.show();

    cvWaitKey();
    return 0;
}
