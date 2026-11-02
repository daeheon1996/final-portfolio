#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "DefectInspector.hpp"

namespace fs = std::filesystem;

int main() {
    try {
        // 1. Initialize Inspector & Paths
        DefectInspector inspector{cv::Rect(340, 400, 180, 180), 50.0, 2.0};

        fs::path input_folder = "data/metal_nut/test/scratch";
        fs::path output_folder = "output/batch";

        // 2. Validate Input Directory
        if(!fs::exists(input_folder) || !fs::is_directory(input_folder)){
            std::cerr << "Input folder not found\n";
            return 1;
        }

        // 3. Create Output Directory & CSV Log
        fs::create_directories(output_folder);

        std::ofstream log_file("output/inspection_log.csv");

        if(!log_file.is_open()){
            std::cerr << "CSV file opening failed\n";
            return 1;
        }

        log_file.exceptions(std::ios::badbit | std::ios::failbit);
        log_file << "Filename,DefectCount,Judgment\n";

        // 4. Collect Image Paths
        std::vector<fs::path> image_paths;

        for(const auto& entry : fs::directory_iterator(input_folder)){
            if(!entry.is_regular_file()){
                continue;
            }

            if(entry.path().extension() == ".png"){
                image_paths.push_back(entry.path());
            }
        }

        // 5. Sort by Filename
        std::sort(image_paths.begin(), image_paths.end());

        // 6. Initialize Batch Counters
        int processed_count = 0;
        int ok_count = 0;
        int ng_count = 0;
        int failed_count = 0;

        // 7. Batch Inspection
        for(const auto& path : image_paths){
            std::cout << "\nProcessing: " << path.filename() << '\n';

            try {
                // 7.1 Load Image
                cv::Mat original = cv::imread(path.string());

                if(original.empty()){
                    std::cerr << "Image loading failed\n";
                    failed_count++;
                    continue;
                }

                // 7.2 Run Inspection
                InspectionResult result = inspector.inspect(original);

                // 7.3 Save Result Image
                fs::path save_path = output_folder / path.filename();

                if(!cv::imwrite(save_path.string(), result.image)){
                    std::cerr << "Image saving failed: " << save_path << '\n';
                    failed_count++;
                    continue;
                }

                // 7.4 Print Inspection Result
                std::cout << "Defects: " << result.defect_count << '\n';
                std::cout << "Judgment: " << (result.is_defective ? "NG" : "OK") << '\n';

                // 7.5 Write CSV Log
                log_file << path.filename().string() << ","
                         << result.defect_count << ","
                         << (result.is_defective ? "NG" : "OK") << '\n';

                // 7.6 Update Batch Counters
                if(result.is_defective){
                    ng_count++;
                } else {
                    ok_count++;
                }

                processed_count++;

            } catch(const std::ios_base::failure& err){
                std::cerr << "CSV logging failed: " << err.what() << '\n';
                return 1;

            } catch(const std::exception& err){
                std::cerr << "Inspection failed: " << err.what() << '\n';
                failed_count++;
            }
        }

        // 8. Finalize CSV Log
        log_file.close();

        // 9. Print Batch Summary
        std::cout << "\n===== Batch Summary =====\n";
        std::cout << "Total Images: " << image_paths.size() << '\n';
        std::cout << "Processed: " << processed_count << '\n';
        std::cout << "OK: " << ok_count << '\n';
        std::cout << "NG: " << ng_count << '\n';
        std::cout << "Failed: " << failed_count << '\n';

    } catch(const std::exception& err){
        std::cerr << "Error: " << err.what() << '\n';
        return 1;
    }

    return 0;
}