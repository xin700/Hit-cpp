//
// Created by xin on 2024/7/26.
//

#ifndef IMAGEHANDLER_H
#define IMAGEHANDLER_H

#include <opencv2/opencv.hpp>

class imageHandler
{
private:
public:
    static cv::Mat enhanceImage(cv::Mat image);
    static void showImage(const cv::Mat& image);
};

#endif //IMAGEHANDLER_H
