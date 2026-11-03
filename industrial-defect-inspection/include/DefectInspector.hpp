
#pragma once

#include <opencv2/core.hpp>

// Inspection Result
struct InspectionResult {
    cv::Mat image;
    int defect_count{0};
    bool is_defective{false};
};

// Defect Inspection Pipeline
class DefectInspector {
private:
    cv::Rect roi;
    double min_area;
    double min_aspect_ratio;

public:
    // Configuration
    DefectInspector(cv::Rect roi, double min_area, double min_aspect_ratio);
    void show_config() const;
    void set_min_area(double value);

    // Inspection
    InspectionResult inspect(const cv::Mat& original) const;
};
