#ifndef KALMANTRACKER_H
#define KALMANTRACKER_H
#include "FrameCapture.h"
#include "BoundingBox.h"

class KalmanTracker
{
    private:
        cv::KalmanFilter kf;
        cv::Mat state;
        cv::Mat measurement;
        int id;
        int framesLost;
        int initialized;
        // Added to remember the size of the tracked object
        float lastWidth;
        float lastHeight;
    public:
        void init(Boundingbox& initialDetection , int id);
        Boundingbox predict();
        void update(Boundingbox& detection);
        bool isLost();
        int getID();
};

#endif // KALMANTRACKER_H
