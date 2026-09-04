#include "FrameCapture.h"

bool FrameCapture::init(std::string source)
{
    if(source == "0")
    {
        cap.open(0);
    }else{
        cap.open(source);
    };
    if(!cap.isOpened())
    {
        std::cerr << "Error opening the webcam" << std::endl;
        return false;
    }

    this -> frameWidth = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    this -> frameHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    this -> fps = cap.get(cv::CAP_PROP_FPS);
    this -> isOpen = true;

    return true;
};

bool FrameCapture::getFrame(cv::Mat& frame)
{
    if(!isOpen)
    {
        return false;
    };
    cap.read(frame);
    if(frame.empty())
    {
        return false;
    }
    return true;
}

int FrameCapture::getWidth()
{
    return frameWidth;
}

int FrameCapture::getHeight()
{
    return frameHeight;
}

double FrameCapture::getFPS()
{
    return fps;
}

void FrameCapture::release()
{
    cap.release();
    isOpen = false;
}