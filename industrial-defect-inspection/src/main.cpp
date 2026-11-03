
#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <opencv2/opencv.hpp>
#include "DefectInspector.hpp"

namespace fs = std::filesystem;

int main() {
    try {
        // Initialize Inspector and Paths
        DefectInspector inspector{cv::Rect(340, 400, 180, 180), 50.0, 2.0};

        fs::path input_folder = "data/metal_nut/test";
        fs::path output_folder = "output/batch";

        // Validate Input Directory
        if(!fs::is_directory(input_folder)){
            throw std::runtime_error("Input folder not found: " + input_folder.string());
        }

        // Prepare Output Directory and CSV Log
        fs::create_directories(output_folder);

        std::ofstream log_file("output/inspection_log.csv");

        if(!log_file.is_open()){
            throw std::runtime_error("CSV file opening failed");
        }

        log_file.exceptions(std::ios::badbit | std::ios::failbit);
        log_file << "Filename,DefectCount,Judgment\n";

        // Collect Image Paths
        std::vector<fs::path> image_paths;

        for(const auto& folder : {"good", "scratch"}){
            fs::path category_path = input_folder / folder;

            if(!fs::is_directory(category_path)){
                throw std::runtime_error("Category folder not found: " + category_path.string());
            }

            for(const auto& entry : fs::directory_iterator(category_path)){
                if(entry.is_regular_file() && entry.path().extension() == ".png"){
                    image_paths.push_back(entry.path());
                }
            }
        }

        std::sort(image_paths.begin(), image_paths.end());

        // Initialize Inspection Counters
        int processed_count = 0;
        int ok_count = 0;
        int ng_count = 0;
        int failed_count = 0;

        // Initialize Confusion Matrix
        int tp = 0;
        int tn = 0;
        int fp = 0;
        int fn = 0;

        // Batch Inspection
        for(const auto& path : image_paths){
            std::cout << "\nProcessing: " << path.filename() << '\n';

            bool actual_defective = (path.parent_path().filename() == "scratch");

            try {
                // Load Image
                cv::Mat original = cv::imread(path.string());

                if(original.empty()){
                    throw std::runtime_error("Image loading failed");
                }

                // Run Inspection
                InspectionResult result = inspector.inspect(original);

                // Save Annotated Image
                fs::path save_path = output_folder / (path.parent_path().filename().string() + "-" + path.filename().string());

                if(!cv::imwrite(save_path.string(), result.image)){
                    throw std::runtime_error("Image saving failed");
                }

                // Write CSV Log
                log_file << save_path.filename().string() << ","
                         << result.defect_count << ","
                         << (result.is_defective ? "NG" : "OK") << '\n';

                // Update Inspection Counters
                if(result.is_defective){
                    ng_count++;
                } else {
                    ok_count++;
                }

                // Update Confusion Matrix
                if(actual_defective && result.is_defective){
                    tp++;
                } else if(!actual_defective && !result.is_defective){
                    tn++;
                } else if(!actual_defective && result.is_defective){
                    fp++;
                } else {
                    fn++;
                }

                processed_count++;

                std::cout << "Defects: " << result.defect_count << '\n';
                std::cout << "Judgment: " << (result.is_defective ? "NG" : "OK") << '\n';

            } catch(const std::ios_base::failure& err){
                std::cerr << "CSV logging failed: " << err.what() << '\n';
                return 1;

            } catch(const std::exception& err){
                std::cerr << "Inspection failed: " << path << " - " << err.what() << '\n';
                failed_count++;
            }
        }

        // Finalize CSV Log
        log_file.close();

        // Print Batch Summary
        std::cout << "\n===== Batch Summary =====\n";
        std::cout << "Total Images: " << image_paths.size() << '\n';
        std::cout << "Processed: " << processed_count << '\n';
        std::cout << "OK: " << ok_count << '\n';
        std::cout << "NG: " << ng_count << '\n';
        std::cout << "Failed: " << failed_count << '\n';

        // Print Confusion Matrix
        std::cout << "\n===== Confusion Matrix =====\n";
        std::cout << "TP: " << tp << '\n';
        std::cout << "TN: " << tn << '\n';
        std::cout << "FP: " << fp << '\n';
        std::cout << "FN: " << fn << '\n';

        // Calculate Performance Metrics
        double accuracy = 0.0;
        double precision = 0.0;
        double recall = 0.0;
        double f1_score = 0.0;

        int total = tp + tn + fp + fn;

        if(total > 0){
            accuracy = static_cast<double>(tp + tn) / total;
        }

        if(tp + fp > 0){
            precision = static_cast<double>(tp) / (tp + fp);
        }

        if(tp + fn > 0){
            recall = static_cast<double>(tp) / (tp + fn);
        }

        if(precision + recall > 0.0){
            f1_score = 2.0 * precision * recall / (precision + recall);
        }

        // Print Performance Metrics
        std::cout << "\n===== Performance Metrics =====\n";
        std::cout << "Accuracy: " << accuracy * 100.0 << "%\n";
        std::cout << "Precision: " << precision * 100.0 << "%\n";
        std::cout << "Recall: " << recall * 100.0 << "%\n";
        std::cout << "F1-score: " << f1_score * 100.0 << "%\n";

    } catch(const std::exception& err){
        std::cerr << "Error: " << err.what() << '\n';
        return 1;
    }

    return 0;
}
