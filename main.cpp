// #include <iostream>
// #include <fileHandler.h>
// #include <imageHandler.h>
// #include <minCircleSolver.h>
// #include <lineSolver.h>
// #include <chrono>
//
// signed main()
// {
//     auto start = std::chrono::high_resolution_clock::now();
//
//     auto image = cv::imread("/Users/xin/Desktop/HIT/240719/线4/20240719184015984.bmp");
//
//
//     auto lineData = lineSolver::solveLine(image);
//     std::cout << "线距丝径: " << std::get<0>(lineData) << " μm" << std::endl;
//     std::cout << "包络外径: " << std::get<1>(lineData) << " μm" << std::endl;
//     std::cout << "出刃高度: " << std::get<2>(lineData) << " μm" << std::endl;
//
//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double, std::milli> duration = end - start;
//     std::cout << "运行时间: " << duration.count() << " 毫秒" << std::endl;
//
//     return 0;
// }


#include <opencv2/opencv.hpp>
#include <iostream>
#include <imageHandler.h>

#include "dataMatrixSolver.h"

int main() {
    cv::Mat image = cv::imread("/Users/xin/Desktop/HIT/测试图像20240719/006331.bmp", cv::IMREAD_GRAYSCALE);
    if (image.empty()) {
        std::cerr << "Could not read the image" << std::endl;
        return 1;
    }
    // image = imageHandler::enhanceImage(image);
    image = dataMatrixSolver::solveDataMatrix(image.clone());
    cv::imshow("image",image);
    cv::waitKey(0);
    // F:
    //
    // cv::Point2i p[4] = {};
    // for(int i=0;i<image_width;++i)
    //     for(int j=0;j<image_height;++j)
    //         if(white_image.at<uchar>(i, j) == 0)
    //         {
    //             p[0] = {i, j};
    //             break;
    //         }
    // for(int i=image_width-1;i>=0;--i)
    //     for(int j=0;j<image_height;++j)
    //         if(white_image.at<uchar>(i, j) == 0)
    //         {
    //             p[1] = {i, j};
    //             break;
    //         }
    // for(int j=0;j<image_height;++j)
    //     for(int i=0;i<image_width;++i)
    //         if(white_image.at<uchar>(i, j) == 0)
    //         {
    //             p[2] = {i, j};
    //             break;
    //         }
    // for(int j=image_height-1;j>=0;--j)
    //     for(int i=0;i<image_width;++i)
    //         if(white_image.at<uchar>(i, j) == 0)
    //         {
    //             p[3] = {i, j};
    //             break;
    //         }
    // for(auto point : p)
    // {
    //     cv::circle(white_image, point, 5, cv::Scalar(0), 25, 8, 0);
    //     std::cout << point << std::endl;
    // }
    // cv::imshow("Shi-Tomasi Corners", white_image);
    // cv::waitKey(0);


    return 0;
}

