#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "DefectInspector.hpp"

namespace fs = std::filesystem;

int main() {
    DefectInspector inspector{cv::Rect(340, 400, 180, 180), 50.0, 2.0};

    fs::path input_folder = "data/metal_nut/test/scratch";
    fs::path output_folder = "output/batch";
    
    try {
        if(!fs::exists(input_folder) || !fs::is_directory(input_folder)){
            std::cerr << "Input folder not found\n";
            return 1;
        }  

        fs::create_directories(output_folder);

        // 1. Collect Image Paths
        std::vector<fs::path> image_paths;

        for(const auto& entry : fs::directory_iterator(input_folder)) {
            if(!entry.is_regular_file()) {
                continue;
            }

            if(entry.path().extension() == ".png"){
                image_paths.push_back(entry.path());
            }
        }

        // 2. Sort by Filename
        std::sort(image_paths.begin(), image_paths.end());

        // 3. Batch Inspection
        int processed_count = 0;

        for(const auto& path : image_paths){
            std::cout << "\nProcessing: " << path.filename() << '\n';

            cv::Mat original = cv::imread(path.string());

            if(original.empty()) {
                std::cerr << "Image loading failed\n";
                continue;
            }

            try {
                cv::Mat result = inspector.inspect(original);
                fs::path save_path = output_folder / path.filename();

                if(!cv::imwrite(save_path.string(), result)) {
                    std::cerr << "Image saving failed: " << save_path << '\n';
                    continue;
                }

                processed_count++;
            } catch (const std::exception& err){
                std::cerr << "Inspection failed: " <<  err.what() << '\n';
            }
        }

        std::cout << "\nBatch Processing Finished\n";
        std::cout << "Processed Images: " << processed_count << '\n';
    }  catch(const std::exception& err){
        std::cerr << "Error: " << err.what() << '\n';
        return 1;
    }

    return 0;
}