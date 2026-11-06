# Industrial 2D Defect Inspection System

## 1. Project Overview

C++20과 OpenCV를 활용하여 **MVTec AD Metal Nut 데이터셋의 표면 Scratch 결함을 검사하는 프로그램**입니다.

Image Processing 기반 결함 후보 검출부터 Batch Processing, OK/NG 판정, 검사 결과 기록 및 성능 평가까지 구현했습니다.

본 프로젝트는 **Machine Vision 검사 프로그램의 전체 동작 과정 구현과 이해**에 중점을 두었기에, 실제 Scratch 위치 검출과 정상/불량 구분 성능에는 한계가 있습니다.

### Tech Stack

| 항목 | 내용 |
|---|---|
| Language | C++20 |
| Computer Vision | OpenCV 4.10 |
| Build / OS | CMake / Ubuntu Linux |
| Dataset | MVTec AD — Metal Nut |
| Inspection Method | Fixed ROI 기반 Rule-based Image Processing |
| Output | 검사 결과 이미지, CSV Log, 성능 평가 지표 |


## 2. System Architecture

검사 실행 및 결과 관리를 담당하는 `main.cpp`와 영상처리 및 결함 검출을 담당하는 `DefectInspector` 클래스로 구성됩니다.

| 구성 파일 | 주요 역할 |
|---|---|
| `main.cpp` | Batch Processing, 결과 저장 및 성능 평가 |
| `DefectInspector.hpp` | 검사 클래스 및 결과 데이터 구조 정의 |
| `DefectInspector.cpp` | Image Processing, 결함 후보 검출 및 OK/NG 판정 |

### Application Workflow

```text
이미지 경로 수집 및 로드
          ↓
Batch Inspection 실행
          ↓
Image Processing 및 결함 후보 검출
          ↓
OK/NG 판정 및 결과 저장
          ↓
Confusion Matrix 및 성능 평가
```

## 3. Image Processing Pipeline

`DefectInspector::inspect()`에서 실행되는 영상처리 과정입니다.

```text
Input BGR Image
       ↓
Fixed ROI 추출
       ↓
Grayscale → Gaussian Blur
       ↓
Morphological Top-Hat
       ↓
Otsu Thresholding
       ↓
External Contour Detection
       ↓
Contour Filtering
  • Contour Area ≥ 50
  • Bounding Box Width / Height ≥ 2.0
       ↓
원본 이미지에 Bounding Box 표시
       ↓
결함 후보 수 > 0 → NG / 그 외 → OK
```

- **Top-Hat:** 주변보다 밝은 국소 구조를 강조
- **Otsu Thresholding:** 처리된 ROI 영상의 픽셀 분포를 기준으로 이진화 임계값을 자동 결정
- **Contour Filtering:** Area와 Aspect Ratio 조건을 모두 만족하는 후보만 유지합니다.
- **Bounding Box:** ROI 내부 좌표를 원본 이미지 좌표로 변환한 후 검출 영역을 표시합니다.


## 4. Inspection Results

검사 결과 이미지는 `output/batch/`에 저장하고, `output/inspection_log.csv`에는 `Filename`, `DefectCount`, `Judgment`를 기록합니다.

### Good Sample

![Good Sample Inspection](docs/images/good-result.png)

### Scratch Sample

![Scratch Sample Inspection](docs/images/scratch-result.png)


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


## 6. Limitations
- **Fixed ROI:** 이미지별 Scratch 위치가 달라 일부 결함이 검사 영역에서 제외됨
- **False Positive:** 정상 표면 패턴을 결함 후보로 잘못 검출
- **Rule-based Filtering:** Area 및 Aspect Ratio 기준만으로 실제 Scratch와 정상 패턴을 구분하는 데 한계


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


