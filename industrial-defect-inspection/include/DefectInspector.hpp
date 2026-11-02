#pragma once

#include <opencv2/core.hpp>

// Inspection Result Data
struct InspectionResult {
    cv::Mat image;
    int defect_count{0};
    bool is_defective{false};
};

// Defect Inspection Pipeline
class DefectInspector {
private:
    // Inspection Parameters
    cv::Rect roi;
    double min_area;
    double min_aspect_ratio;

public:
    // Constructor
    DefectInspector(cv::Rect roi, double min_area, double min_aspect_ratio);

    // Configuration
    void show_config() const;
    void set_min_area(double value);

    // Run Inspection
    InspectionResult inspect(const cv::Mat& original) const;
};