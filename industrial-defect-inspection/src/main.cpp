#include <iostream>
#include <opencv2/opencv.hpp>

int main(){
    // 1. Load Image
    cv::Mat image = cv::imread("data/normal/test.png");

    // 2. Validate Image
    if(image.empty()) {
        std::cerr << "Failed to load image\n";
        return 1;
    }

    // 3. Print Image Information
    std::cout << "Width = " << image.cols << '\n';
    std::cout << "Height = " << image.rows << '\n';
    std::cout << "Channels = " << image.channels() << '\n';

    // 4. Save Image
    if(!cv::imwrite("output/result.png", image)) {
        std::cerr << "Failed to save image\n";
        return 1;
    }

    std::cout << "Image saved successfully\n";

    return 0;
}