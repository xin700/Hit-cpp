//
// Created by xin on 2024/7/26.
//

#include "imageHandler.h"

#include <utility>

cv::Mat imageHandler::enhanceImage(cv::Mat image)
{
    cv::Mat enhancedImage = image.clone();
    cv::cvtColor(enhancedImage, enhancedImage, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(enhancedImage, enhancedImage);
    // cv::adaptiveThreshold(enhancedImage, enhancedImage, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 15, 10);
    cv::cvtColor(enhancedImage, enhancedImage, cv::COLOR_GRAY2BGR);
    return enhancedImage;
}

void imageHandler::showImage(const cv::Mat& image)
{
    cv::imshow("image", image);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

