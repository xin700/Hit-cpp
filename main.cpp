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

    auto image_path = "/Users/xin/Desktop/HIT/金刚线/2.bmp";
    auto image = cv::imread(image_path);
    if (image.empty())
    {
        std::cerr << "Error: Image is empty!" << std::endl;
        return -1;
    }
    minCircleSolver::solveMinCircle(image);


#ifdef SHOW_TIME
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::cout << "计算时间: " << duration.count() << " 毫秒" << std::endl;
#endif


    return 0;
}