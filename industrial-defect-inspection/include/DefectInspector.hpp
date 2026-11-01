#pragma once
#include <opencv2/core.hpp>

class DefectInspector {
private:
    cv::Rect roi;
    double min_area;
    double min_aspect_ratio;

public:
    DefectInspector(cv::Rect roi, double min_area, double min_aspect_ratio);
    
    void show_config() const;
    void set_min_area(double value);

    cv::Mat inspect(const cv::Mat& original) const;
};