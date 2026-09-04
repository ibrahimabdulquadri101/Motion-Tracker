#include "BackgroundModel.h"
#include <opencv2/opencv.hpp>

void BackgroundModel::init(cv::Mat& firstFrame, float learningRate, int threshold)
{
    cv::Mat gray;
    cv::cvtColor(firstFrame, gray, cv::COLOR_BGR2GRAY);
    gray.convertTo(background, CV_32F);

    this->learningRate = learningRate;
    this->threshold = threshold;
    this->initialized = true;
    
}