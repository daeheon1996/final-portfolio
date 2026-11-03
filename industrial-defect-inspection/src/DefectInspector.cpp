
#include "DefectInspector.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <opencv2/imgproc.hpp>

DefectInspector::DefectInspector(cv::Rect roi, double min_area, double min_aspect_ratio)
    : roi{roi}, min_area{min_area}, min_aspect_ratio{min_aspect_ratio} {

    // Validate Inspection Parameters
    if(roi.width <= 0 || roi.height <= 0 ||
       !std::isfinite(min_area) || !std::isfinite(min_aspect_ratio) ||
       min_area <= 0 || min_aspect_ratio <= 0){
        throw std::invalid_argument("Invalid inspection parameters");
    }
}

void DefectInspector::show_config() const {
    std::cout << "ROI = " << roi.x << ", " << roi.y << '\n';
    std::cout << "Min Area = " << min_area << '\n';
    std::cout << "Min Aspect Ratio = " << min_aspect_ratio << '\n';
}

void DefectInspector::set_min_area(double value) {
    if(!std::isfinite(value) || value <= 0){
        std::cout << "Invalid Area\n";
        return;
    }

    min_area = value;
}

InspectionResult DefectInspector::inspect(const cv::Mat& original) const {
    // 1. Validate Input Image and ROI
    if(original.empty()){
        throw std::invalid_argument("Empty input image");
    }

    if(original.type() != CV_8UC3){
        throw std::invalid_argument("Expected 8-bit BGR image");
    }

    if((roi & cv::Rect(0, 0, original.cols, original.rows)) != roi){
        throw std::invalid_argument("Invalid ROI");
    }

    // 2. Extract ROI
    cv::Mat cropped = original(roi).clone();

    // 3. Grayscale Conversion and Gaussian Blur
    cv::Mat gray;
    cv::Mat blurred;

    cv::cvtColor(cropped, gray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(gray, blurred, cv::Size(5,5), 0);

    // 4. Morphological Top-Hat
    cv::Mat tophat;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15,15));
    cv::morphologyEx(blurred, tophat, cv::MORPH_TOPHAT, kernel);

    // 5. Otsu Thresholding
    cv::Mat defect_binary;
    cv::threshold(tophat, defect_binary, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // 6. Contour Detection
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(defect_binary, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // 7. Contour Filtering and Bounding Box Visualization
    cv::Mat result = original.clone();
    int detected_count = 0;

    for(const auto& contour : contours){
        // 7.1 Filter by Contour Area
        double area = cv::contourArea(contour);

        if(area < min_area){
            continue;
        }

        // 7.2 Filter by Aspect Ratio
        cv::Rect box = cv::boundingRect(contour);
        double aspect_ratio = static_cast<double>(box.width) / box.height;

        if(aspect_ratio < min_aspect_ratio){
            continue;
        }

        // 7.3 Convert ROI Coordinates to Image Coordinates
        box.x += roi.x;
        box.y += roi.y;

        // 7.4 Draw Bounding Box and Count Detection
        cv::rectangle(result, box, cv::Scalar(0, 0, 255), 2);
        detected_count++;
    }

    // 8. OK/NG Judgment
    bool has_defect = detected_count > 0;

    if(has_defect){
        cv::putText(result, "NG", cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 0, 255), 3);
    } else {
        cv::putText(result, "OK", cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(0, 255, 0), 3);
    }

    // 9. Print Inspection Statistics
    std::cout << "Total contours: " << contours.size() << '\n';
    std::cout << "Filtered contours: " << detected_count << '\n';
    std::cout << "Inspection Result: " << (has_defect ? "NG" : "OK") << '\n';

    // 10. Return Inspection Result
    InspectionResult inspection_result;
    inspection_result.image = result;
    inspection_result.defect_count = detected_count;
    inspection_result.is_defective = has_defect;

    return inspection_result;
}
