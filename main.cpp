#include <iostream>
#include <fileHandler.h>
#include <imageHandler.h>
#include <minCircleSolver.h>
#include <lineSolver.h>
#include <chrono>

signed main()
{
    auto start = std::chrono::high_resolution_clock::now();

    auto image = cv::imread("/Users/xin/Desktop/HIT/240719/线4/20240719184015984.bmp");


    auto lineData = lineSolver::solveLine(image);
    std::cout << "线距丝径: " << std::get<0>(lineData) << " μm" << std::endl;
    std::cout << "包络外径: " << std::get<1>(lineData) << " μm" << std::endl;
    std::cout << "出刃高度: " << std::get<2>(lineData) << " μm" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "运行时间: " << duration.count() << " 毫秒" << std::endl;

    return 0;
}
