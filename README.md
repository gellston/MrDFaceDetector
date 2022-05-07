# MrD FaceDetector 1.0

Development Environment
=======================
 - **Visual Studio 2022**
 - **C++ 17**
 - **Windows 10 SDK 10.0.18362.0**
 - **Toolset v143**

# Results
<img src="https://github.com/gellston/MrDFaceDetector/blob/main/Results/1.jpg?raw=true" width=1024/>
<img src="https://github.com/gellston/MrDFaceDetector/blob/main/Results/2.jpg?raw=true" width=1024/>

# Example
```cpp

#include <iostream>
#include <filesystem>
#include <chrono>


#include <faceDetector.h>
#include <opencv2/opencv.hpp>

int main()
{
    //모델 초기화 : 모델 경로, 0.5(iou threshold), 0.5(score threshold)
    mrd::faceDetector detector("C://Github//DeepLearningStudy//trained_model//TRAIN_WIDERFACE(MobileNetV2CenterNet).pt", 0.5, 0.5);
    
    
    std::string path = "C://Github//MrDFaceDetector//Samples";
    for (const auto& entry : std::filesystem::directory_iterator(path)) {

        cv::Mat originalImage = cv::imread(entry.path().u8string(), cv::IMREAD_COLOR);
        //예측 : 이미지 시작 주소, 이미지 너비, 이미지 높이, 이미지 채널 (3채널만 가능)
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


```