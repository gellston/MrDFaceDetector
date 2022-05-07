
#include <iostream>
#include <filesystem>
#include <chrono>


#include <faceDetector.h>
#include <opencv2/opencv.hpp>

int main()
{
    
    mrd::faceDetector detector("C://Github//DeepLearningStudy//trained_model//TRAIN_WIDERFACE(MobileNetV2CenterNet).pt", 0.5, 0.5);
    std::string path = "C://Github//MrDFaceDetector//Samples";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {

        cv::Mat originalImage = cv::imread(entry.path().u8string(), cv::IMREAD_COLOR);
        auto boxes = detector.prediction(originalImage.data, originalImage.cols, originalImage.rows, 3);
        for (auto box : boxes) {
            cv::rectangle(originalImage, cv::Rect(box->x(), box->y(), box->width(), box->height()), cv::Scalar(0, 255, 0), 1);
        }
        cv::namedWindow("result", cv::WINDOW_AUTOSIZE);
        cv::resizeWindow("result", cv::Size(1280, 1024));
        cv::imshow("result", originalImage);
        cv::waitKey(1000);
    }
}

