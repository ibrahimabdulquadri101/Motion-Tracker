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
    public:
        bool init(std::string source);
        bool getFrame(cv::Mat& frame);
        int getWidth();
        int getHeight();
        double getFPS();
        void release();
};

#endif // FRAMECAPTURE_H
