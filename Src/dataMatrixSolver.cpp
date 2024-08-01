//
// Created by xin on 2024/7/31.
//

#include <dataMatrixSolver.h>
#include <minCircleSolver.h>
#include <imageHandler.h>


std::vector<cv::Point2i> dataMatrixSolver::solveDataMatrix(cv::Mat image)
{
    double max_area = -1;
    std::vector<cv::Point2i> max_points;
    auto points = dataMatrixSolver::getDataMaxtrixPoints(image);
    max_area = minCircleSolver::real_distance(points[0], points[1]) * minCircleSolver::real_distance(points[1], points[2]);
    max_points = points;

    image = imageHandler::enhanceImage(image);

    points = dataMatrixSolver::getDataMaxtrixPoints(image);
    if(minCircleSolver::real_distance(points[0], points[1]) * minCircleSolver::real_distance(points[1], points[2]) > max_area)
    {
        max_area = minCircleSolver::real_distance(points[0], points[1]) * minCircleSolver::real_distance(points[1], points[2]);
        max_points = points;
    }
    return max_points;
}


std::vector<cv::Point2i> dataMatrixSolver::getDataMaxtrixPoints(cv::Mat image)
{
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    cv::Mat white_image = cv::Mat::ones(image.size(), image.type()) * 255;

    std::vector<cv::Point2f> corners;
    int maxCorners = 100;
    double qualityLevel = 0.01;
    double minDistance = 8;
    int blockSize = 3;
    bool useHarrisDetector = false;
    double k = 0.04;

    cv::goodFeaturesToTrack(image, corners, maxCorners, qualityLevel, minDistance, cv::Mat(), blockSize, useHarrisDetector, k);

    for (size_t i = 0; i < corners.size(); i++) {
        cv::circle(white_image, corners[i], 2, cv::Scalar(0), 23, 8, 0);
    }
#ifdef SHOW
    cv::imshow("image",image);
    cv::waitKey(0);
    cv::imshow("Shi-Tomasi Corners", white_image);
    cv::waitKey(0);
#endif

    for(int i=1;i<=DILATE_AMOUNT;++i)
        cv::dilate(white_image, white_image, cv::Mat());
    cv::threshold(white_image, white_image, 200, 255, cv::THRESH_BINARY);

#ifdef SHOW
    cv::imshow("image", white_image);
    cv::waitKey(0);
#endif
    int image_height = image.rows, image_width = image.cols;


    cv::Mat graph = white_image.clone();
    std::vector<cv::Point2i> links[(int)1e3];
    std::map<std::pair<int,int>,int> visited;
    int link_amount = 0;
    int black_amount = 0;
    for(int i=0;i<image_width;i++)
    {
        for(int j=0;j<image_height;++j)
        {
            black_amount += graph.at<uchar>(i,j) == 0;
            if(visited.find(std::make_pair(i,j)) != visited.end())
                continue;
            if(graph.at<uchar>(i,j) == 0)
            {
                dataMatrixSolver::searchFrom(std::make_pair(i,j),
                    graph, links, visited, link_amount);
            }
        }
    }
    white_image = cv::Mat::ones(image.size(), image.type()) * 255;
    std::sort(links, links + link_amount, [](const std::vector<cv::Point2i>& a, const std::vector<cv::Point2i>& b) -> bool
    {
        return a.size() > b.size();
    });
    for(auto &point : links[0])
    {
        std::swap(point.x,point.y);
        // cv::circle(white_image, point, 1, cv::Scalar(0), 1, 8, 0);
    }
    auto answerData =  dataMatrixSolver::getCouners(links[0]);
    std::vector<cv::Point2i> points;
    points.push_back(std::get<0>(answerData));
    points.push_back(std::get<1>(answerData));
    points.push_back(std::get<2>(answerData));
    points.push_back(std::get<3>(answerData));
    return points;
}

void dataMatrixSolver::searchFrom(std::pair<int, int> start_point, cv::Mat& graph,
    std::vector<cv::Point2i> (&links)[1000], std::map<std::pair<int, int>, int>& visited, int &this_link_index)
{
    std::pair<int,int> directions[] = {{0,1},{1,0},{0,-1},{-1,0}};
    std::queue<std::pair<int,int>> q;

    visited[start_point] = this_link_index;

    q.push(start_point);

    while(q.size())
    {
        auto now = q.front(); q.pop();
        links[this_link_index].emplace_back(now.first, now.second);
        for(auto direction : directions)
        {
            auto next_point = std::make_pair(now.first + direction.first, now.second + direction.second);
            if(next_point.first < 0 or next_point.first >= graph.rows or next_point.second < 0 or next_point.second >= graph.cols)
                continue;
            if(visited.find(next_point) != visited.end())
                continue;
            if(graph.at<uchar>(next_point.first, next_point.second) == 0)
            {
                visited[next_point] = this_link_index;
                q.push(next_point);
            }
        }
    }
    if(links[this_link_index].size() < 10)
        links[this_link_index].clear();
    else
        this_link_index ++;

}

std::tuple<cv::Point2i,cv::Point2i,cv::Point2i,cv::Point2i>
        dataMatrixSolver::getCouners(const std::vector<cv::Point2i>& points)
{
    std::tuple<cv::Point2i,cv::Point2i,cv::Point2i,cv::Point2i> answerData;

    int max_X = -1e9, max_Y = -1e9, min_X = 1e9, min_Y = 1e9;
    cv::Point2i max_X_point = {0,0}, max_Y_point = {0,0}, min_X_point = {0,0}, min_Y_point = {0,0};

    for(auto point : points)
    {
        if(point.x > max_X)
        {
            max_X = point.x;
            max_X_point = point;
        }
        if(point.x < min_X)
        {
            min_X = point.x;
            min_X_point = point;
        }
        if(point.y > max_Y)
        {
            max_Y = point.y;
            max_Y_point = point;
        }
        if(point.y < min_Y)
        {
            min_Y = point.y;
            min_Y_point = point;
        }

    }
    answerData = {min_X_point, min_Y_point, max_X_point, max_Y_point};

    return killIt(answerData);
}

std::tuple<cv::Point2i, cv::Point2i, cv::Point2i, cv::Point2i>
        dataMatrixSolver::killIt(const std::tuple<cv::Point2i, cv::Point2i, cv::Point2i, cv::Point2i>& answerData)
{
    cv::Point2i p[] = {std::get<0>(answerData), std::get<1>(answerData), std::get<2>(answerData), std::get<3>(answerData)};
    for(int i=0;i<=3;++i)
    {
        for(int j=0;j<=3;++j)
        {
            for(int k=0;k<=3;++k)
            {
                if(i == j or j == k or k == i) continue;
                auto angle = minCircleSolver::angle(p[i], p[j], p[k]);
                auto distance1 = minCircleSolver::real_distance(p[i], p[j]);
                auto distance2 = minCircleSolver::real_distance(p[j], p[k]);

                if(fabs(angle - 90.0) < 10
                    and fabs(distance1 - distance2) < std::max(distance1,distance2) * 0.1)
                {
                    return {p[i], p[j], p[k], calculateFourthPoint(p[i], p[j], p[k])};
                }
            }
        }
    }
    double max_distance = -1;
    std::pair<cv::Point2i,cv::Point2i> max_pair;

    for(int i=0;i<=3;++i)
        for(int j=i+1;j<=3;++j)
            if(minCircleSolver::real_distance(p[i],p[j]) > max_distance)
            {
                max_distance = minCircleSolver::real_distance(p[i],p[j]);
                max_pair = {p[i],p[j]};
            }
    return getSqurePoints(max_pair);
}

cv::Point2i dataMatrixSolver::calculateFourthPoint(const cv::Point2i& p1, const cv::Point2i& p2, const cv::Point2i& p3)
{
    cv::Point2i p4;
    p4.x = p3.x + (p1.x - p2.x);
    p4.y = p3.y + (p1.y - p2.y);
    return p4;
}

std::tuple<cv::Point2i, cv::Point2i, cv::Point2i, cv::Point2i>
        dataMatrixSolver::getSqurePoints(std::pair<cv::Point2i, cv::Point2i> max_pair)
{
    // Extract the two diagonal points
    cv::Point2i pt1 = max_pair.first;
    cv::Point2i pt2 = max_pair.second;

    // Calculate the midpoint
    cv::Point2f midpoint((pt1.x + pt2.x) / 2.0, (pt1.y + pt2.y) / 2.0);

    // Calculate half the length of the diagonal vector
    float u = (pt2.x - pt1.x) / 2.0;
    float v = (pt2.y - pt1.y) / 2.0;

    // Calculate the other two points
    cv::Point2i pt3(midpoint.x - v, midpoint.y + u);
    cv::Point2i pt4(midpoint.x + v, midpoint.y - u);

    // Return the points as a tuple
    std::tuple<cv::Point2i, cv::Point2i, cv::Point2i, cv::Point2i> answerData = std::make_tuple(pt1, pt3, pt2, pt4);

    return answerData;
}
