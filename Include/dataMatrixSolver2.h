//
// Created by xin on 2024/8/8.
//

#ifndef DATAMATRIXSOLVER2_H
#define DATAMATRIXSOLVER2_H

#include <opencv2/opencv.hpp>
namespace dataMatrixSolver2
{
    double distance(cv::Point2f p1, cv::Point2f p2);
    cv::Mat solve(cv::Mat image);
}

#endif //DATAMATRIXSOLVER2_H
