#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

int main() {
    // 0. Load Image
    cv::Mat original = cv::imread("data/metal_nut/test/scratch/000.png");

    if (original.empty()) {
        std::cerr << "Image loading failed\n";
        return 1;
    }

    // 1. Image Preprocessing
    cv::Mat gray;
    cv::Mat blurred;

    cv::cvtColor(original, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);

    // 2. Thresholding
    cv::Mat binary;
    cv::threshold(blurred, binary, 127, 255, cv::THRESH_BINARY);

    // 3. Morphology (Opening)
    cv::Mat opened;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));

    cv::morphologyEx(binary, opened, cv::MORPH_OPEN, kernel);

    // 4. Contour Detection
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(opened, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 5. Bounding Box Visualization
    cv::Mat result = original.clone();
    int detected_count = 0;

    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);

        if (area < 100.0) {
            continue;
        }

        cv::Rect box = cv::boundingRect(contour);
        cv::rectangle(result, box, cv::Scalar(0, 0, 255), 2);

        detected_count++;
        std::cout << "Area: " << area << '\n';
    }

    // 6. Display Results
    std::cout << "Total contours: " << contours.size() << '\n';
    std::cout << "Filtered contours: " << detected_count << '\n';

    cv::imshow("Original", original);
    cv::imshow("Grayscale", gray);
    cv::imshow("Gaussian Blur", blurred);
    cv::imshow("Binary", binary);
    cv::imshow("Opening", opened);
    cv::imshow("Bounding Boxes", result);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}