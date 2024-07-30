//
// Created by xin on 2024/7/30.
//

#ifndef LINESOLVER_H
#define LINESOLVER_H

#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <vector>
#include <random>
#include <algorithm>
#include <iostream>

namespace lineSolver
{
#define BORDER 49
#define MAP4LEN 0.8625
#define CHECKER_LENGTH 10
#define CHECKR_RADIUS_FRONT 0.018
#define CHECKR_RADIUS_BACK  0.01
    std::tuple<double,double,double> solveLine(cv::Mat image_clone);
    cv::Mat rotate2horizon(cv::Mat image);
    cv::Mat rotate4angle(cv::Mat img, double m);
}


#endif //LINESOLVER_H
