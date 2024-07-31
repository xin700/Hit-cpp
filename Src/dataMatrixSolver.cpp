//
// Created by xin on 2024/7/31.
//

#include <dataMatrixSolver.h>

cv::Mat dataMatrixSolver::solveDataMatrix(cv::Mat image)
{
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
    auto [p1, p2, p3, p4] = dataMatrixSolver::getCouners(links[0]);
    cv::circle(image, p1, 5, cv::Scalar(0), 25, 8, 0);
    cv::circle(image, p2, 5, cv::Scalar(0), 25, 8, 0);
    cv::circle(image, p3, 5, cv::Scalar(0), 25, 8, 0);
    cv::circle(image, p4, 5, cv::Scalar(0), 25, 8, 0);

    return image;
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

    return answerData;
}
