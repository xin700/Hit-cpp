//
// Created by xin on 2024/7/29.
//

#include <minCircleSolver.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <imageHandler.h>

#define ANGLE_LIMIT 30

void minCircleSolver::solveMinCircle(cv::Mat image)
{
    std::vector<int> path;

    cv::Mat image_clone;
    image.copyTo(image_clone);
    cv::cvtColor(image,image,cv::COLOR_BGR2GRAY);
    int start_index;
    int image_width = image.cols,image_height = image.rows;
    int kernel_length = image_width / 50;
    cv::Mat white_image = cv::Mat::ones(image.size(),CV_8UC1)*255;
    cv::line(white_image,cv::Point(10,10),cv::Point(10+kernel_length,10),cv::Scalar(0),5);
    int step = kernel_length >> 1;
    std::vector<cv::Point2i> rawPoints,Points;
    std::map<std::pair<uchar,uchar>,bool> visited;
    std::map<cv::Point2i,int ,PointCompare> belong;
    std::bitset<(int)1e4> vis;
    std::queue<int> vis_que;
    std::vector<int> vis_index[(int)2e3];


    cv::threshold(image,image,120,255,cv::THRESH_BINARY|cv::THRESH_OTSU);
    cv::erode(image,image,cv::getStructuringElement(cv::MORPH_RECT,cv::Size(3,3)));

    // cv::imshow("image",image);
    // cv::waitKey(0);

    for(int i=1;i<image_width;i += 2)
    {
        auto point = findPoint_i(image,i);
        if(!point.empty())
        {
            rawPoints.insert(rawPoints.end(),point.begin(),point.end());
        }
    }
    for(int j=step/2;j<image_height;j += 2)
    {
        auto point = findPoint_j(image,j);
        if(!point.empty())
        {
            rawPoints.insert(rawPoints.end(),point.begin(),point.end());
        }
    }

    std::sort(rawPoints.begin(),rawPoints.end(),
        [](const cv::Point2i &x,const cv::Point2i &y)-> bool {return x.x < y.x;});

    for(int i=0;i<rawPoints.size();++i)
    {
        if(visited[std::make_pair(rawPoints[i].x,rawPoints[i].y)]) continue;
        visited[std::make_pair(rawPoints[i].x,rawPoints[i].y)] = true;
        bool tag = false;
        for(int j=std::max(0,i-step);j<std::min(j+step,(int)rawPoints.size());++j)
        {
            if(i != j and real_distance(rawPoints[i],rawPoints[j]) < step * 0.7)
            {
                tag = true;
                visited[std::make_pair(rawPoints[j].x,rawPoints[j].y)] = true;
            }
        }
        if(!tag) continue;
        for(int j=i+1;j<i+100;++j)
        {
            if(distance(rawPoints[i],rawPoints[j]) > kernel_length)
                break;
            visited[std::make_pair(rawPoints[j].x,rawPoints[j].y)] = true;
        }


        Points.emplace_back(rawPoints[i]);

    }



#ifdef SHOW
    cv::Mat white2 = cv::Mat::ones(image.size(),CV_8UC1)*255;
    // for(auto point : rawPoints)
    // {
    //     cv::circle(white2,point,8,cv::Scalar(0),cv::FILLED);
    // }
    // cv::imshow("123",white2);
    // cv::waitKey(0);
    white2 = cv::Mat::ones(image.size(),CV_8UC1)*255;
    for(auto point : Points)
    {
        cv::circle(white2,point,8,cv::Scalar(0),cv::FILLED);
    }
    // cv::imshow("123",white2);
    // cv::waitKey(0);



    std::cout << "Points size: " << Points.size() << std::endl;
    white_image = cv::Mat::ones(image.size(),CV_8UC1)*255;
    for(auto point : Points)
    {
        cv::circle(white_image,point,8,cv::Scalar(0),cv::FILLED);
    }
    cv::imshow("white_image",white_image);
    cv::waitKey(0);
    white_image = cv::Mat::ones(image.size(),CV_8UC1)*255;
#endif

#ifndef DEBUG
    cv::Point2i last_point = Points[0],now_point;
    for(int i=1;i<Points.size();++i)
    {
        if(distance(last_point,Points[i]) < kernel_length * 2)
        {
            now_point = Points[i];
            break;
        }
    }
    cv::Point2i direction = now_point - last_point;
    cv::Point2i last_direction = direction;

    for(int i=0;i<Points.size();++i)
        belong[Points[i]] = i;
    start_index = belong[now_point];

    auto find_index = [&](int index,int val) -> bool
    {
        for(auto v : vis_index[index])
            if (v == val)
                return false;
        return true;
    };

    int temp_limit = 0,temp_limit_tag = false;
    while(true)
    {

        cv::Point2i next_range = now_point + direction;
        int index = belong[now_point];
        int min_distance = 0x7f7f7f7f,min_index = -1;
        double min_angle = min_distance;
        double minn = min_distance;

        if(path.size() and path[path.size()-1] != index)
            path.push_back(index);
        else if(path.empty())
            path.push_back(index);
        for(int i=std::max(index-20,0);i<=std::min(index+20,(int)Points.size()-1);++i)
        {
            // std::cout << angle(last_point,now_point,Points[i]) << std::endl;
            if(vis[i]) continue;
            auto check_angle = angle(last_point,now_point,Points[i]);
            // if(distance(Points[i],next_range) < min_distance
            //     and check_angle < ANGLE_LIMIT
            //     and distance(Points[i],next_range) < kernel_length * 3
            //     and find_index(i,index))
            // {
            //     min_distance = distance(Points[i],next_range);
            //     min_index = i;
            // }
            if(distance(Points[i],next_range) + check_angle * 2 < minn
                and (check_angle < ANGLE_LIMIT or (temp_limit_tag and check_angle > temp_limit))
                and distance(Points[i],next_range) < kernel_length * 2
                and find_index(i,index))
            {
                minn = distance(Points[i],next_range) + check_angle * 2 ;
                // std::cout << "distance: " << distance(Points[i],next_range) << " angle: " << check_angle << std::endl;
                min_index = i;
            }

        }
        // std::cout << "min_index: " << min_index << std::endl;
        if(!~min_index)
        {

            temp_limit_tag = true;
            temp_limit += 10;
            if(temp_limit > 100) break;
            continue;
        }
        temp_limit_tag = false,temp_limit = 0;

        last_point = now_point;
        now_point = Points[min_index];
        vis[min_index] = 1;
        direction = now_point - last_point;
        direction = direction * 0.7;
        // add_edge(index,min_index);

        vis_que.push(min_index);
        if(vis_que.size() > 10)
        {
            vis[vis_que.front()] = 0;
            vis_que.pop();

            while(!vis_que.empty())
            {
                int front = vis_que.front();
                vis_index[index].push_back(front);
                vis_que.pop();
            }
            for(auto front : vis_index[index])
                vis_que.push(front);
        }

        if(now_point.x >= image_width - 5) break;

#ifdef SHOW
        // cv::circle(white_image,now_point,8,cv::Scalar(0),cv::FILLED);
        // cv::line(white_image,last_point,now_point,cv::Scalar(0),2);
        // cv::imshow("123",white_image);
        // cv::waitKey(3);
#endif
    }

    auto minCircle = killCircle(Points,path);
    auto center_point = cv::Point2i((Points[std::get<0>(minCircle)].x + Points[std::get<1>(minCircle)].x) / 2,
                                    (Points[std::get<0>(minCircle)].y + Points[std::get<1>(minCircle)].y) / 2);
    std::cout << "center_point: " << center_point << std::endl;
    cv::circle(image_clone,center_point,std::get<2>(minCircle)/2 ,cv::Scalar(0,255,0),5);
    std::cout << "minCircle: " << std::get<0>(minCircle) << " " << std::get<1>(minCircle) << " " << std::get<2>(minCircle) << std::endl;

#ifdef SHOW
    // cv::imshow("image",image_clone);
    // cv::waitKey(0);
    // cv::destroyAllWindows();
#endif

#endif

#ifdef DEBUG
    imageHandler::showImage(white_image);
#endif
}


std::vector<cv::Point2i> minCircleSolver::findPoint_i(cv::Mat &image,int x)
{
    std::vector<cv::Point2i> ret;
    for(int i=x,j=0;j<image.rows;++j)
    {
        if(image.at<uchar>(j,i)==0)
        {
            ret.emplace_back(i,j);
        }
    }
    return ret;
}


std::vector<cv::Point2i> minCircleSolver::findPoint_j(cv::Mat &image,int y)
{
    std::vector<cv::Point2i> ret;
    for(int i=0,j=y;i<image.cols;++i)
    {
        if(image.at<uchar>(j,i)==0)
        {
            ret.emplace_back(i,j);
        }
    }
    return ret;
}

int minCircleSolver::distance(const cv::Point2i &x,const cv::Point2i &y)
{
    return abs(x.x - y.x) + abs(x.y - y.y);
}

double minCircleSolver::real_distance(const cv::Point2i &x,const cv::Point2i &y)
{
    return std::sqrt(1.0 * (x.x - y.x) * (x.x - y.x) + 1.0 * (y.y - x.y) * (y.y - x.y));
}


double minCircleSolver::angle(cv::Point2i x, cv::Point2i y, cv::Point2i z)
{
    constexpr double pi = 3.1415926;
    double angle1 = atan2(1.0 * (y.y - x.y) , (y.x - x.x)) * 180 / pi;
    double angle2 = atan2(1.0 * (z.y - y.y) , (z.x - y.x)) * 180 / pi;
    double diff = fmod(angle2 - angle1, 360.0);
    if (diff < -180.0) {
        diff += 360.0;
    } else if (diff > 180.0) {
        diff -= 360.0;
    }
    return fabs(diff);
}

std::tuple<int,int,double> minCircleSolver::killCircle(const std::vector<cv::Point2i> &Points,const std::vector<int> &path)
{
    std::tuple<int,int,double> minCircle(0,0,1000000);
    std::vector<int> degrees[(int)1e4];
    for(int i=0;i<path.size();++i)
    {
        degrees[path[i]].push_back(i);
    }
    cv::Mat white_image = cv::Mat::ones(cv::Size(5120,5120),CV_8UC1)*255;

    for(int i=0;i<Points.size();++i)
    {
        if(degrees[i].size() > 1)
        {

            for(int j=0;j<degrees[i].size() - 1;++j)
            {
                auto start_index = degrees[i][j];

                for(int k = start_index;k < degrees[i][j+1];++ k)
                {
                    double farest = -1;
                    int farest_index = -1;
                    for(int t = start_index;t < degrees[i][j+1];++ t)
                    {
                        auto p1 = Points[path[k]],p2 = Points[path[t]];

                        if(real_distance(p1,p2) > farest)
                        {
                            farest = real_distance(p1,p2);
                            farest_index = t;
                        }
                    }

                    if(farest < std::get<2>(minCircle) and farest > 100)
                    {
                        minCircle = std::make_tuple(path[k],path[farest_index],farest);
                    }

                }
            }
        }
    }

    return minCircle;
}
