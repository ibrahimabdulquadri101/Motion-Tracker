#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "FrameCapture.h"
#include "BackgroundModel.h"
#include "BlobDetector.h"

void runBlobDetectorTest(const std::string& source, bool isWebcam)
{
    std::cout << "\n==========================================" << std::endl;
    std::cout << (isWebcam ? "     TESTING BLOB DETECTOR (WEBCAM)       " 
                           : "   TESTING BLOB DETECTOR (VIDEO FILE)     ") << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Source: " << source << std::endl;

    FrameCapture fc;
    if (!fc.init(source))
    {
        std::cerr << "[FAIL] Failed to open source: " << source << std::endl;
        return;
    }

    // Step 1: Read the first frame for background initialization
    cv::Mat firstFrame;
    if (!fc.getFrame(firstFrame))
    {
        std::cerr << "[FAIL] Could not read first frame." << std::endl;
        return;
    }

    std::cout << "[INFO] Source opened: " << fc.getWidth() << "x" << fc.getHeight() 
              << " @ " << fc.getFPS() << " FPS" << std::endl;

    // Initialize Background Model
    float learningRate = 0.03f;
    int threshold = 25;
    BackgroundModel bgModel;
    bgModel.init(firstFrame, learningRate, threshold);

    // Initialize Blob Detector (minArea = 500, dilationSize = 3)
    int minBlobArea = 500;
    int dilationSize = 3;
    BlobDetector blobDetector(minBlobArea, dilationSize);

    std::cout << "[INFO] Background Model & Blob Detector initialized." << std::endl;
    std::cout << "  - Min Blob Area: " << minBlobArea << " px" << std::endl;
    std::cout << "  - Dilation Size: " << dilationSize << std::endl;
    std::cout << "  - Press 'q' or ESC on preview window to exit.\n" << std::endl;

    cv::Mat frame;
    int frameCount = 1;
    int delay = isWebcam ? 1 : std::max(1, static_cast<int>(1000.0 / std::max(1.0, fc.getFPS())));

    // Step 2: Loop reading frames
    while (fc.getFrame(frame))
    {
        frameCount++;

        // Pass frame through background model to get motion mask
        cv::Mat mask = bgModel.apply(frame);

        // Pass mask into blob detector to find bounding boxes of moving objects
        std::vector<Boundingbox> blobs = blobDetector.detect(mask);

        // Print number of blobs detected per frame
        std::cout << "Frame #" << frameCount << " -> Blobs Detected: " << blobs.size() << std::endl;

        // Draw bounding boxes on the original frame
        for (size_t i = 0; i < blobs.size(); ++i)
        {
            const auto& b = blobs[i];
            // Draw rectangle around detected blob
            cv::Rect boxRect(b.x, b.y, b.width, b.height);
            cv::rectangle(frame, boxRect, cv::Scalar(0, 255, 0), 2);

            // Draw label with confidence / index
            std::string label = "Blob #" + std::to_string(i + 1) + " (" + std::to_string(b.width * b.height) + "px)";
            cv::putText(frame, label, cv::Point(b.x, std::max(20, b.y - 8)), 
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
        }

        // Draw HUD banner with blob count
        std::string hudText = "Blobs: " + std::to_string(blobs.size()) + " | Frame: " + std::to_string(frameCount);
        cv::putText(frame, hudText, cv::Point(20, 35), 
                    cv::FONT_HERSHEY_SIMPLEX, 0.8, 
                    blobs.empty() ? cv::Scalar(0, 200, 255) : cv::Scalar(0, 255, 0), 2);

        // Display original frame with overlays and binary mask
        cv::imshow("Motion & Blob Tracker", frame);
        cv::imshow("Foreground Mask", mask);

        char key = static_cast<char>(cv::waitKey(delay));
        if (key == 'q' || key == 'Q' || key == 27) // 27 = ESC
        {
            std::cout << "\n[INFO] Exiting test loop." << std::endl;
            break;
        }
    }

    fc.release();
    cv::destroyAllWindows();
    std::cout << "\n[SUCCESS] Blob detector test completed." << std::endl;
}

int main(int argc, char** argv)
{
    std::string source = "assets/test_video.mp4";
    bool isWebcam = false;

    if (argc > 1)
    {
        std::string arg = argv[1];
        if (arg == "--webcam" || arg == "-w" || arg == "0")
        {
            source = (argc > 2) ? argv[2] : "0";
            isWebcam = true;
        }
        else if (arg == "--help" || arg == "-h")
        {
            std::cout << "Usage:" << std::endl;
            std::cout << "  " << argv[0] << "                     # Test on video file (assets/test_video.mp4)" << std::endl;
            std::cout << "  " << argv[0] << " <video_path>        # Test on custom video file" << std::endl;
            std::cout << "  " << argv[0] << " --webcam [index]    # Test live webcam (default index 0)" << std::endl;
            return 0;
        }
        else
        {
            source = arg;
            isWebcam = false;
        }
    }

    runBlobDetectorTest(source, isWebcam);

    return 0;
}
