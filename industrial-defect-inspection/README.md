# Industrial 2D Defect Inspection System

## 1. Project Overview

프로젝트명: 결함 검사 시스템

주요 기능:
1. 금속 너트의 Scratch 여부를 Image Processing 기반으로 검사하고 OK/NG 판정
2. Batch Processing을 통해 총 45장의 이미지를 자동으로 검사
3. 검사결과 저장 및 성능 평가

프로젝트 목적:
- Machine Vision SW 개발 직무에 필요한 기본적인 결함 검사 프로그램의 동작 과정 이해


### Tech Stack

| 항목 | 내용 |
|---|---|
| Language | C++20 |
| Computer Vision | OpenCV 4.10 |
| Build / OS | CMake / Ubuntu Linux |
| Dataset | MVTec AD — Metal Nut |


<br>

## 2. System Architecture

| 구성 파일 | 주요 역할 |
|---|---|
| `main.cpp` | 검사 실행 -> 결과 저장 -> 성능 평가 |
| `DefectInspector.hpp` | DefectInspector class 정의 |
| `DefectInspector.cpp` | Image Processing -> Defect Detection -> OK/NG Judgement |

### Application Workflow

```text
1. Image Loading
         ↓
2. Batch Inspection
         ↓
3. DefectInspector::inspect()
   - Image Processing
   - Defect Detection
   - OK/NG Judgment
         ↓
4. Result Management
   - Inspection Image 저장
   - CSV Logging
         ↓
5. Performance Evaluation
   - Confusion Matrix
   - Classification Metrics
```
 
<br>

## 3. Image Processing Pipeline

```text
1. Input Image
       ↓
2. Fixed ROI Extraction
       ↓
3. Grayscale → Gaussian Blur
       ↓
4. Morphological Top-Hat
       ↓
5. Otsu Thresholding
       ↓
6. Contour Detection
       ↓
7. Contour Filtering & Bounding Box
  • Contour Area ≥ 50
  • Bounding Box Width / Height ≥ 2.0
       ↓
8. (Defect count > 0) ? NG : OK 판정
```

- **Top-Hat:** 주변보다 밝은 구조를 강조
- **Otsu Thresholding:** 처리된 ROI 영상의 픽셀 분포를 기준으로 이진화 임계값을 자동 결정
- **Contour Filtering:** Area와 Aspect Ratio 조건을 모두 만족하는 후보만 유지
- **Bounding Box:** ROI 내부 좌표를 원본 이미지 좌표로 변환한 후 검출 영역을 표시  

<br>

## 4. Inspection Results

검사 결과 이미지는 `output/batch/`에 저장하고, `output/inspection_log.csv`에는 `Filename`, `DefectCount`, `Judgment`를 기록합니다.

### Good Sample

![Good Sample Inspection](docs/images/good-result.png)

### Scratch Sample

![Scratch Sample Inspection](docs/images/scratch-result.png)

////////////  ### CSV log file screentshot  

<br>

## 5. Performance Evaluation

MVTec AD Metal Nut 데이터셋의 45장의 이미지(Good 22장, Scratch 23장)를 대상으로 OK/NG 분류 성능을 평가했습니다.

### Confusion Matrix
| 실제 분류 / 예측 | NG | OK |
|---|---:|---:|
| Scratch | 23 (TP) | 0 (FN) |
| Good | 22 (FP) | 0 (TN) |

- **TP (True Positive):** 실제 불량을 불량으로 판정
- **TN (True Negative):** 실제 정상을 정상으로 판정
- **FP (False Positive):** 실제 정상을 불량으로 오판
- **FN (False Negative):** 실제 불량을 정상으로 오판

### Classification Metrics

| 지표 | 설명 | 결과 |
|---|---|---:|
| Accuracy | 전체 이미지 중 올바르게 판정한 비율 | 51.11% |
| Precision | NG 판정 중 실제 불량의 비율 | 51.11% |
| Recall | 실제 불량 중 NG로 판정한 비율 | 100.00% |
| F1 Score | Precision과 Recall의 조화평균 | 67.65% |

**평가 결과:** 정상 이미지 22장를 포함한 전체 이미지 45장 모두 NG로 판정되어 실제 Scratch 위치 검출에는 실패하였습니다. 따라서 Recall 100%는 실제 Scratch 위치를 정확하게 검출했다는 의미가 아니며, 정상/불량 구분 성능에도 한계가 있음을 확인했습니다.  

<br>

## 6. Limitations
- **Fixed ROI:** 이미지별 Scratch 위치가 달라 일부 결함이 검사 영역에서 제외됨
- **False Positive:** 정상 표면 패턴을 결함 후보로 잘못 검출
- **Rule-based Filtering:** Area 및 Aspect Ratio 기준만으로 실제 Scratch와 정상 패턴을 구분하는 데 한계  

<br>

## 7. Project Structure

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
├── data/          #gitignore
├── output/        #gitignore     
└── README.md
```
 
<br>

## 8. Build & Run

MVTec AD Metal Nut 데이터셋을 다운로드 받고,  `data/metal_nut/` 폴더를 별도 생성한 후 다운받은 데이터를 이 폴더에 저장해야합니다. 
프로그램은 `test/good/` 및 `test/scratch/` 폴더의 PNG 파일을 읽습니다.

```bash
cmake -S . -B build
cmake --build build
./build/inspection
```

**생성 결과**

- `output/batch/` — Bounding Box와 OK/NG 판정이 표시된 이미지
- `output/inspection_log.csv` — 이미지별 검사 결과
- Console — Batch Summary, Confusion Matrix, Classification Metrics


