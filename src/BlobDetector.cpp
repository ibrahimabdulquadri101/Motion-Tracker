#include "BlobDetector.h"
#include "FrameCapture.h"
#include <opencv2/opencv.hpp>
#include <opencv2/geometry/2d.hpp>
#include <algorithm>
#include "BoundingBox.h"

cv::Mat BlobDetector::preprocess(cv::Mat& mask)
{
    cv::Mat result;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(dilationSize,dilationSize));
    cv::erode(mask, result, kernel);
    cv::dilate(result, result, kernel);
    cv::GaussianBlur(result, result, cv::Size(5, 5), 1.5);

    if (result.channels() > 1) {
        cv::cvtColor(result, result, cv::COLOR_BGR2GRAY);
    }
    if (result.type() != CV_8U) {
        result.convertTo(result, CV_8U);
    }

    // re-threshold to produce a clean binary mask (use Otsu to avoid needing a member)
    cv::threshold(result, result, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    return result;
}

std::vector<Boundingbox> BlobDetector::detect(cv::Mat& mask)
{
    auto cleanMask = preprocess(mask);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(cleanMask,contours,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    std::vector<Boundingbox> results;
    for(auto& contour : contours)
    {
        double area = cv::contourArea(contour);
        if(area < minBlobArea)
        {
            continue;
        }
        auto rect = cv::boundingRect(contour);
        auto confidence = std::min(1.0, area/5000.0);
        Boundingbox box = {rect.x, rect.y, rect.width, rect.height, (float)confidence};
        results.push_back(box);
    };
    return results;
}