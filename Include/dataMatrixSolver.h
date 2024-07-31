//
// Created by xin on 2024/7/31.
//

#ifndef DATAMATRIXSOLVER_H
#define DATAMATRIXSOLVER_H

#include <opencv2/opencv.hpp>

namespace dataMatrixSolver
{
#define DILATE_AMOUNT 14
    cv::Mat solveDataMatrix(cv::Mat image);
    void searchFrom(std::pair<int,int>start_point, cv::Mat& graph,
        std::vector<cv::Point2i> (&links)[1000], std::map<std::pair<int,int>,int>& visited, int &this_link_index);
    std::tuple<cv::Point2i,cv::Point2i,cv::Point2i,cv::Point2i> getCouners(const std::vector<cv::Point2i>& points);
}

#endif //DATAMATRIXSOLVER_H
