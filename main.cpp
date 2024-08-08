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

    // auto file = "/Users/xin/Desktop/HIT/240719/线1/20240719180030150.bmp";
    // auto image = cv::imread(file);
    // auto answerData = lineSolver::solveLine(image);
    // std::cout << "线距丝径: " << std::get<0>(answerData) << " μm" << std::endl;
    // std::cout << "包络外径: " << std::get<1>(answerData) << " μm" << std::endl;
    // std::cout << "出刃高度: " << std::get<2>(answerData) << " μm" << std::endl;

    // auto file = "/Users/xin/Desktop/HIT/金刚线/2.jpeg";
    // auto image = cv::imread(file);
    // minCircleSolver::solveMinCircle(image);



    // auto file = "/Users/xin/Desktop/HIT/测试图像20240719/005385.bmp";
    // auto files = fileHandler::getFilesInDirectory("/Users/xin/Desktop/HIT/测试图像20240719");
    // for(auto file : files)
    // {
    //
    //     auto image = cv::imread(file);
    //     auto points = dataMatrixSolver::solveDataMatrix(image);
    //     auto radius = minCircleSolver::real_distance(points[0],points[2]) / 2;
    //     std::cout << "radius: " << radius << std::endl;
    //     if(radius > MAX_CIRCLE_RADIUS)
    //         radius = 65;
    //     if(radius < MIN_CIRCLE_RADIUS)
    //         radius = 50;
    //     cv::circle(image,(points[0] + points[2]) / 2, (int)radius, cv::Scalar(0,255,0), 2);
    //
    //
    //     cv::imshow("image",image);
    //     cv::waitKey(0);
    // }





#ifdef SHOW_TIME
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "计算时间: " << duration.count() << " 毫秒" << std::endl;
#endif


    return 0;
}