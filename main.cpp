#include <iostream>
#include <fileHandler.h>
#include <imageHandler.h>
#include <minCircleSolver.h>
#include <chrono>

signed main()
{
    auto start = std::chrono::high_resolution_clock::now();

    auto image = cv::imread("/Users/xin/Desktop/HIT/金刚线/2.bmp");
    std::cout << "width: " << image.cols << " height: " << image.rows << std::endl;

    minCircleSolver::solveMinCircle(image);


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "运行时间: " << duration.count() << " 毫秒" << std::endl;

    return 0;
}
