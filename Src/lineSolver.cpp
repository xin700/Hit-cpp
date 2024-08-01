//
// Created by xin on 2024/7/30.
//

#include <lineSolver.h>

std::tuple<double, double, double> lineSolver::solveLine(cv::Mat image)
{
    std::tuple<double,double,double> answerData = {0,0,0};

    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

    if (image.empty())
    {
        std::cerr << "Error: Image is empty!" << std::endl;
        return answerData;
    }

#ifdef SHOW
    cv::imshow("image", image);
    cv::waitKey(0);
#endif
    image = rotate2horizon(image);
#ifdef SHOW
    cv::imshow("image",image);
    cv::waitKey(0);
    auto image_clone = image.clone();
    cv::cvtColor(image_clone, image_clone, cv::COLOR_GRAY2BGR);
#endif
    std::cout << "图像水平矫正完毕" << std::endl;

    int image_height = image.rows, image_width = image.cols;

    std::cout << "正在扫描图像" << std::endl;

    std::vector<double> lengths;

    auto scan = [&](int x) -> std::vector<double>
    {
        std::vector<double> ret;
        ret.reserve(image_height);
        for (int y = 0; y < image_height; ++y)
        {
            ret.push_back(image.at<uchar>(y, x));
            // std::cout << (int)image.at<uchar>(y, x) << ' ';
        }
        // std::cout << std::endl;
        return ret;
    };

    int sta,end;
    auto get_length = [&](const std::vector<double>& values) -> double
    {
        double left = 0,right = 0;
        sta = end = 0;
        for(int i=0;i<values.size();i++)
            if(values[i + 1] > BORDER and values[i] <= BORDER)
            {
                left = i + 1.0 * (BORDER - values[i]) / (values[i + 1] - values[i]);
                sta = i;
                break;
            }

        for(int i=values.size();i>=0;--i)
            if(values[i-1] > BORDER and BORDER >= values[i])
            {
                right = i + 1.0 * (BORDER - values[i]) / (values[i - 1] - values[i]);
                end = i;
                break;
            }
        return (right - left) * MAP4LEN;
    };

    for(int i=1;i<image_width-1;i+=2)
    {
        auto values = scan(i);
        lengths.push_back(get_length(values));
#ifdef SHOW
        cv::circle(image_clone, cv::Point(i,sta), 1, cv::Scalar(0,255,0), 3);
        cv::circle(image_clone, cv::Point(i,end), 1, cv::Scalar(0,255,0), 3);
#endif
    }
#ifdef SHOW
    cv::imshow("image_clone",image_clone);
    cv::waitKey(0);
#endif

    std::sort(lengths.begin(),lengths.end());


    double length_min = 0,length_max = 0;

    for(int i=1;i<lengths.size() - CHECKER_LENGTH;++i)
        if(lengths[i + CHECKER_LENGTH] - lengths[i] < CHECKR_RADIUS_FRONT * CHECKER_LENGTH)
        {
            length_min = lengths[i];
            break;
        }

    for(int i=lengths.size() - 1;i>=CHECKER_LENGTH;--i)
        if(lengths[i] - lengths[i - CHECKER_LENGTH] < CHECKR_RADIUS_BACK * CHECKER_LENGTH)
        {
            length_max = lengths[i];
            break;
        }

    answerData = {length_min,length_max,(length_max - length_min) / 2};

    return answerData;
}

cv::Mat lineSolver::rotate2horizon(cv::Mat image)
{

    auto image_original = image.clone();

    cv::Mat sharpen_kernel = (cv::Mat_<float>(3, 3) << -1, -1, -1, -1, 9, -1, -1, -1, -1);

    // 锐化图像
    cv::Mat enhanced_image;
    cv::filter2D(image, enhanced_image, -1, sharpen_kernel);

    // 自适应阈值
    cv::adaptiveThreshold(image, image, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 11, 2);

    int width = image.cols;
    int height = image.rows;
    std::vector<int> x_list;

    // 生成随机x坐标
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, width - 1);
    for (int i = 0; i < width / 2; ++i) {
        x_list.push_back(dis(gen));
    }

    // 排序并去重
    std::sort(x_list.begin(), x_list.end());
    x_list.erase(std::unique(x_list.begin(), x_list.end()), x_list.end());

    std::vector<std::tuple<int, int, int>> data_list;
    std::vector<cv::Point> top_points;
    std::vector<cv::Point> bottom_points;

    // 计算顶点和底点
    for (int x : x_list) {
        int y_top = 0, y_bottom = 0;
        for (int y = 0; y < height; ++y) {
            if (image.at<uchar>(y, x) != 255) {
                y_top = y;
                break;
            }
        }

        for (int y = height - 1; y >= 0; --y) {
            if (image.at<uchar>(y, x) != 255) {
                y_bottom = y;
                break;
            }
        }

        int length = std::abs(y_top - y_bottom);
        data_list.emplace_back(y_top, y_bottom, length);
        top_points.emplace_back(x, y_top);
        bottom_points.emplace_back(x, y_bottom);
    }

    std::vector<cv::Point> points = top_points;
    int n = points.size();
    Eigen::MatrixXd A(n, 2);
    Eigen::VectorXd y(n);
    for (int i = 0; i < n; ++i) {
        A(i, 0) = points[i].x;
        A(i, 1) = 1.0;
        y(i) = points[i].y;
    }

    Eigen::VectorXd result = A.colPivHouseholderQr().solve(y);
    double m = result(0);
    double c = result(1);

    return rotate4angle(image_original, -m);
}

cv::Mat lineSolver::rotate4angle(cv::Mat img, double m)
{
    double angle = std::atan(m) * (180.0 / CV_PI);
    std::cout << "旋转了" << std::round(angle * 10000) / 10000 << "度达到水平" << std::endl;

    int h = img.rows;
    int w = img.cols;
    cv::Point2f center(w / 2.0f, h / 2.0f);

    // 获取旋转矩阵
    cv::Mat M = cv::getRotationMatrix2D(center, -angle, 1.0);

    cv::Mat rotated_img;
    // 应用仿射变换
    cv::warpAffine(img, rotated_img, M, cv::Size(w, h));

    return rotated_img;

}
