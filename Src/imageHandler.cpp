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

cv::Mat imageHandler::brightenImage(const cv::Mat &image, float alpha, float beta)
{
    int height = image.rows;
    int width = image.cols;
    cv::Mat dst = cv::Mat::zeros(image.size(), image.type());

    for (int row = 0; row < height; row++) {
        for (int col = 0; col < width; col++) {
            if (image.channels() == 3) {
                int b = image.at<cv::Vec3b>(row, col)[0];
                int g = image.at<cv::Vec3b>(row, col)[1];
                int r = image.at<cv::Vec3b>(row, col)[2];

                dst.at<cv::Vec3b>(row, col)[0] = cv::saturate_cast<uchar>(b * alpha + beta);
                dst.at<cv::Vec3b>(row, col)[1] = cv::saturate_cast<uchar>(g * alpha + beta);
                dst.at<cv::Vec3b>(row, col)[2] = cv::saturate_cast<uchar>(r * alpha + beta);

            } else if (image.channels() == 1) {
                float v = image.at<uchar>(row, col);
                dst.at<uchar>(row, col) = cv::saturate_cast<uchar>(v * alpha + beta);
            }
        }
    }
    return dst;
}

