# Industrial 2D Defect Inspection System

A C++20/OpenCV-based prototype for automated surface defect inspection using the MVTec AD Metal Nut dataset.

This project implements a modular inspection application featuring image processing, batch execution, result visualization, CSV logging, and quantitative performance evaluation.

## 1. Tech Stack

- **Language:** C++20
- **Computer Vision:** OpenCV 4.10
- **Build System:** CMake
- **Environment:** Ubuntu Linux
- **Dataset:** MVTec AD (Metal Nut)

## 2. Key Features

- **Image Processing:** Grayscale, Gaussian Blur, Morphological Top-Hat, and Otsu Thresholding
- **Defect Detection:** Contour detection with area and aspect ratio filtering
- **Inspection Software:** C++ class-based inspection pipeline with error handling
- **Batch Processing:** Automated inspection of multiple images
- **Visualization & Logging:** Bounding boxes, OK/NG judgments, and CSV records

## 3. Inspection Pipeline

```text
Input Image
    ↓
ROI Selection
    ↓
Grayscale → Gaussian Blur
    ↓
Morphological Top-Hat
    ↓
Otsu Thresholding
    ↓
Contour Detection
    ↓
Area & Aspect Ratio Filtering
    ↓
Bounding Box Visualization
    ↓
OK / NG Judgment
```

The inspection pipeline is implemented in the `DefectInspector` class using a fixed Region of Interest (ROI).

## 4. Project Structure

```text
industrial-defect-inspection/
├── CMakeLists.txt
├── include/
│   └── DefectInspector.hpp
├── src/
│   ├── main.cpp
│   └── DefectInspector.cpp
├── docs/
│   └── images/
│       ├── good-result.png
│       └── scratch-result.png
├── data/
│   └── metal_nut/
├── output/
│   ├── batch/
│   └── inspection_log.csv
└── README.md
```

The dataset, build artifacts, and generated inspection outputs are excluded from version control.

## 5. Build & Run

**Requirements:** C++20-compatible compiler, CMake, and OpenCV.

Download the MVTec AD Metal Nut dataset separately and place it under `data/metal_nut/` with the original directory structure.

```bash
cmake -S . -B build
cmake --build build
./build/inspection
```

The application reads images from `data/metal_nut/test/good/` and `data/metal_nut/test/scratch/`.

Generated outputs:

- **Annotated Images:** `output/batch/`
- **Inspection Log:** `output/inspection_log.csv`
- **Console Output:** Batch summary, confusion matrix, and classification metrics

## 6. Inspection Examples

The following images show inspection results for normal and scratched metal nuts.

**Good Sample**

![Good Sample Inspection](docs/images/good-result.png)

**Scratch Sample**

![Scratch Sample Inspection](docs/images/scratch-result.png)

Red bounding boxes indicate detected candidate regions rather than verified defects.

## 7. Performance Evaluation

Image-level classification was evaluated on 45 images from the MVTec AD Metal Nut dataset:

- **Good:** 22 images
- **Scratch:** 23 images

### Confusion Matrix

| | Predicted NG | Predicted OK |
|---|---:|---:|
| Actual Scratch | 23 | 0 |
| Actual Good | 22 | 0 |

### Classification Metrics

| Metric | Result |
|---|---:|
| Accuracy | 51.11% |
| Precision | 51.11% |
| Recall | 100.00% |
| F1 Score | 67.65% |

**Interpretation:** All 45 evaluation images were classified as NG. The 100% recall therefore does not indicate reliable defect discrimination, as all normal images were also misclassified.

These results describe the evaluated dataset and should not be interpreted as performance on an independent, previously unseen test set.

## 8. Limitations

- The fixed ROI does not consistently cover defect locations across the dataset.
- Normal surface textures can produce false-positive detections.
- The current rule-based detector does not reliably distinguish scratches from normal surface patterns.

This project demonstrates the implementation and quantitative evaluation of a C++ machine vision inspection prototype, rather than a production-ready defect detection system.