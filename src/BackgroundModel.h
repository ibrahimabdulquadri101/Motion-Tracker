#ifndef BACKGROUNDMODEL_H
#define BACKGROUNDMODEL_H
#include "BoundingBox.h"
#include "FrameCapture.h"

class BackgroundModel 
{
    private:
        cv::Mat background;
        float learningRate;
        int threshold;
        bool initialized;
    public:
        void init(cv::Mat& firstFrame , float learningRate , int threshold);
        cv::Mat apply(cv::Mat& frame);
        void reset();
};

#endif // BACKGROUNDMODEL_H
