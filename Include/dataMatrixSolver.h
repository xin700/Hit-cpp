//
// Created by xin on 2024/7/31.
//

#ifndef DATAMATRIXSOLVER_H
#define DATAMATRIXSOLVER_H

#include <opencv2/opencv.hpp>

namespace dataMatrixSolver
{
#define DILATE_AMOUNT 8
#define THICKNESS 22
#define MIN_CIRCLE_RADIUS 40
#define MAX_CIRCLE_RADIUS 70
    std::vector<cv::Point2i> solveDataMatrix(cv::Mat image);
    std::vector<cv::Point2i> getDataMaxtrixPoints(cv::Mat image);
    std::vector<cv::Point2i> comparePoints(std::vector<std::vector<cv::Point2i>> pointsData);
    void searchFrom(std::pair<int,int>start_point, cv::Mat& graph,
        std::vector<cv::Point2i> (&links)[1000], std::map<std::pair<int,int>,int>& visited, int &this_link_index);
    std::tuple<cv::Point2i,cv::Point2i,cv::Point2i,cv::Point2i> getCouners(const std::vector<cv::Point2i>& points);
    std::tuple<cv::Point2i,cv::Point2i,cv::Point2i,cv::Point2i> killIt(const std::tuple<cv::Point2i,cv::Point2i,cv::Point2i,cv::Point2i> &answerData);
    cv::Point2i calculateFourthPoint(const cv::Point2i& p1, const cv::Point2i& p2, const cv::Point2i& p3);
    std::tuple<cv::Point2i,cv::Point2i,cv::Point2i,cv::Point2i> getSqurePoints(std::pair<cv::Point2i,cv::Point2i> max_pair);

}

#endif //DATAMATRIXSOLVER_H
