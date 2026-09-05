#ifndef BLOBDETECTOR_H
#define BLOBDETECTOR_H
#include "BoundingBox.h"
#include <vector>
#include "FrameCapture.h"


class BlobDetector
{
    private:
        int minBlobArea;
        int dilationSize;
    public:
        BlobDetector(int minArea = 500, int dilSize = 3) 
            : minBlobArea(minArea), dilationSize(dilSize) {}
        std::vector<Boundingbox> detect(cv::Mat& mask);
        cv::Mat preprocess(cv::Mat& mask);
};

#endif // BLOBDETECTOR_H
