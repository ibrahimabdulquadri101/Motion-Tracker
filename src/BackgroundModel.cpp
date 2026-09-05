#include "BackgroundModel.h"
#include <opencv2/opencv.hpp>
#include <cmath>
#include "FrameCapture.h"
void BackgroundModel::init(cv::Mat& firstFrame, float learningRate, int threshold)
{
    cv::Mat gray;
    cv::cvtColor(firstFrame, gray, cv::COLOR_BGR2GRAY);
    gray.convertTo(background, CV_32F);

    this->learningRate = learningRate;
    this->threshold = threshold;
    this->initialized = true;
}

cv::Mat BackgroundModel::apply(cv::Mat& frame)
{
    cv::Mat gray;
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

    cv::Mat gray32;
    gray.convertTo(gray32, CV_32F);

    cv::Mat absDiff;
    cv::absdiff(background, gray32, absDiff);

    cv::Mat mask;
    cv::threshold(absDiff, mask, this->threshold, 255, cv::THRESH_BINARY);
    mask.convertTo(mask, CV_8U);
    cv::accumulateWeighted(gray,background,this -> learningRate);
    return mask;
}

void BackgroundModel::reset()
{
    background.release();
    initialized = false;
}