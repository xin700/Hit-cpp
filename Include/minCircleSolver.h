//
// Created by xin on 2024/7/29.
//

#ifndef MINCIRCLESOLVER_H
#define MINCIRCLESOLVER_H

#include <opencv2/opencv.hpp>
#include <vector>

namespace minCircleSolver
{
    struct PointCompare {
        bool operator()(const cv::Point_<int>& lhs, const cv::Point_<int>& rhs) const {
            return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
        }
    };
    void solveMinCircle(cv::Mat image);
    std::vector<cv::Point2i> findPoint_i(cv::Mat &image,int x);
    std::vector<cv::Point2i> findPoint_j(cv::Mat &image,int x);
    int distance(const cv::Point2i &x,const cv::Point2i &y);
    double real_distance(const cv::Point2i &x,const cv::Point2i &y);
    double angle(cv::Point2i x, cv::Point2i y, cv::Point2i z);
    std::tuple<int,int,double> killCircle(const std::vector<cv::Point2i> &Points,const std::vector<int> &path);

}

#endif //MINCIRCLESOLVER_H
