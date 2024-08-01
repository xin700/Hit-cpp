#include <iostream>
#include <fileHandler.h>
#include <imageHandler.h>
#include <minCircleSolver.h>
#include <lineSolver.h>
#include <chrono>
#include <dataMatrixSolver.h>

signed main()
{
#ifdef SHOW_TIME
    auto start = std::chrono::high_resolution_clock::now();
#endif


    auto image = cv::imread("/Users/xin/Desktop/HIT/测试图像20240719/006057.bmp");
    auto points = dataMatrixSolver::solveDataMatrix(image);
    cv::circle(image,(points[0] + points[2]) / 2, minCircleSolver::real_distance(points[0],points[2]) / 2, cv::Scalar(0,255,0), 2);
    // cv::imshow("image",image);
    // cv::waitKey(0);



#ifdef SHOW_TIME
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "计算时间: " << duration.count() << " 毫秒" << std::endl;
#endif


    return 0;
}