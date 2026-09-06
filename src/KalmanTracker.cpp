#include "KalmanTracker.h"

void KalmanTracker::init(Boundingbox& initialDetection , int id)
{
    kf = cv::KalmanFilter(4,2,0);
    this->measurement = cv::Mat::zeros(2, 1, CV_32F);

    cv::setIdentity(kf.transitionMatrix);
    kf.transitionMatrix.at<float>(0, 2) = 1.0f; // x depends on vx
    kf.transitionMatrix.at<float>(1, 3) = 1.0f;

    kf.measurementMatrix = cv::Mat::zeros(2, 4, CV_32F);
    kf.measurementMatrix.at<float>(0, 0) = 1.0f;
    kf.measurementMatrix.at<float>(1, 1) = 1.0f;

    cv::setIdentity(kf.processNoiseCov, cv::Scalar::all(1e-2));
    cv::setIdentity(kf.measurementNoiseCov, cv::Scalar::all(1e-1));
    cv::setIdentity(kf.errorCovPost, cv::Scalar::all(1));

    float cx = initialDetection.x + (initialDetection.width / 2.0f);
    float cy = initialDetection.y + (initialDetection.height / 2.0f);

    kf.statePost.at<float>(0) = cx;
    kf.statePost.at<float>(1) = cy;
    kf.statePost.at<float>(2) = 0.0f; // initial vx
    kf.statePost.at<float>(3) = 0.0f; // initial vy

    this->lastWidth = initialDetection.width;
    this->lastHeight = initialDetection.height;
    this->id = id;
    this->framesLost = 0;
    this->initialized = true;
}


Boundingbox KalmanTracker::predict()
{
    cv::Mat prediction = kf.predict();
    float cx = prediction.at<float>(0);
    float cy = prediction.at<float>(1);
    
    framesLost++; // Advance lost count (reset if update() is called)

    // Convert center point back to a top-left BoundingBox
    Boundingbox box;
    box.x = static_cast<int>(cx - (lastWidth / 2.0f));
    box.y = static_cast<int>(cy - (lastHeight / 2.0f));
    box.width = static_cast<int>(lastWidth);
    box.height = static_cast<int>(lastHeight);
    box.confidence = 1.0f;
    box.id = id;
    return box;
}


void KalmanTracker::update(Boundingbox& detection)
{
    float cx = detection.x + (detection.width / 2.0f);
    float cy = detection.y + (detection.height / 2.0f);

    measurement.at<float>(0) = cx;
    measurement.at<float>(1) = cy;

    // Correct the Kalman Filter with the real measurement
    kf.correct(measurement);
    
    // Update stored sizes and reset lost counter
    lastWidth = detection.width;
    lastHeight = detection.height;
    framesLost = 0;
}

bool KalmanTracker::isLost()
{
    return framesLost > 10;
}

int KalmanTracker::getID()
{
    return id;
}

