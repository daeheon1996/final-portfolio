#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

int main() {
    // 1. Load Image
    cv::Mat original = cv::imread("data/metal_nut/test/scratch/000.png");

    if (original.empty()) {
        std::cerr << "Image loading failed\n";
        return 1;
    }

    // 2. ROI Selection
    cv::Rect roi(340, 400, 180, 180);
    cv::Mat cropped = original(roi).clone();


    // 3. Image Preprocessing
    cv::Mat gray;
    cv::Mat blurred;

    cv::cvtColor(cropped, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5, 5), 0);
    
    
    // 4. Tophat
    cv::Mat tophat;
    cv::Mat th_kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15,15));
    cv::morphologyEx(blurred, tophat, cv::MORPH_TOPHAT, th_kernel);

    // 5. Tophat thresholding
    cv::Mat defect_binary;
    cv::threshold(tophat, defect_binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // 6. Contour Detection
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(defect_binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 7. Bounding Box & Feature Filtering
    cv::Mat result = original.clone();
    int detected_count = 0;

    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);

        if (area < 50.0) {
            continue;
        }

        cv::Rect box = cv::boundingRect(contour);
        int width = box.width;
        int height = box.height;
        double aspect_ratio = static_cast<double>(width) / height;

        if(aspect_ratio < 2.0){
            continue;
        }
    
        // Convert ROI coordinates to original image coordinates
        box.x += roi.x;
        box.y += roi.y;

        cv::rectangle(result, box, cv::Scalar(0, 0, 255), 2);
        detected_count++;
    }

    // 8. OK/NG Classification
    bool has_defect = detected_count > 0;
    if(has_defect){
        std::cout << "Inspection Result = NG\n";
        cv::putText(result, "NG", cv::Point(30,50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0,0,255), 3);
    } else {
        std::cout << "Inspection Result = OK\n";
        cv::putText(result, "OK", cv::Point(30,50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0,255,0), 3);
    }

    // 9. Display Results
    std::cout << "Total contours: " << contours.size() << '\n';
    std::cout << "Filtered contours: " << detected_count << '\n';

    // cv::imshow("Original", original);
    // cv::imshow("Grayscale", gray);
    // cv::imshow("Gaussian Blur", blurred);
    // cv::imshow("Binary", binary);
    cv::imshow("Bounding Boxes", result);

    // ROI
    // cv::imshow("ROI Selection", roi_display);
    // cv::imshow("Cropped ROI", cropped);

    // tophat
    // cv::imshow("Top hat", tophat);
    cv::imshow("Defect Binary", defect_binary);

    cv::waitKey(0);
    cv::destroyAllWindows();

    return 0;
}