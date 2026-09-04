#ifndef FRAMECAPTURE_H
#define FRAMECAPTURE_H
#include "opencv2/opencv.hpp"
using namespace cv;

class FrameCapture
{
    private:
        cv::VideoCapture cap;
        int frameWidth;
        int frameHeight;
        double fps;
        bool isOpen;
};

#endif // FRAMECAPTURE_H
