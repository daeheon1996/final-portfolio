# Software Development Portfolio

C/C++ 기반의 소프트웨어 개발 프로젝트를 정리한 포트폴리오입니다.
Linux 환경에서의 System Programming부터 C++/OpenCV를 활용한 Machine Vision까지, 실제 동작하는 소프트웨어를 직접 설계하고 구현하는 데 중점을 두었습니다.

## Projects

### 1. Mini Shell | C

Linux 환경에서 동작하는 간단한 Unix-like Shell 구현 프로젝트입니다.

**주요 구현 기능**
- `fork()`, `execvp()`를 활용한 프로세스 생성 및 명령어 실행
- Built-in Commands (`cd`, `pwd`, `exit`)
- `open()`, `dup2()`를 활용한 입출력 Redirection
- `pipe()`를 활용한 단일 Pipeline
- `waitpid()`를 활용한 자식 프로세스 관리
- 명령어 실행 및 파일 처리 관련 Error Handling

**Tech Stack:** C, Linux, GCC, Make

[프로젝트 상세 보기](https://github.com/daeheon1996/final-portfolio/tree/main/mini-shell)

---

### 2. Multithreaded HTTP Server | C

POSIX Threads와 TCP Socket을 활용하여 여러 클라이언트의 요청을 동시에 처리하는 HTTP/1.1 서버 구현 프로젝트입니다.

**주요 구현 기능**
- TCP Socket 기반 Client-Server 통신
- `pthread`를 활용한 Thread-per-Client 구조
- HTTP Request Parsing 및 Response 처리
- Mutex와 Condition Variable을 활용한 Thread Synchronization
- Signal Handling을 활용한 Graceful Shutdown
- Socket I/O 및 예외 상황에 대한 Error Handling

**Tech Stack:** C, Linux, POSIX Threads, TCP/IP, GCC, Make

[프로젝트 상세 보기](https://github.com/daeheon1996/final-portfolio/tree/main/multithreaded-http-server)

---

### 3. Defect Inspection | C++

C++20과 OpenCV를 활용하여 산업용 부품 이미지의 표면 결함을 검사하는 Machine Vision 프로토타입 프로젝트입니다.

MVTec AD Metal Nut 데이터셋을 활용하여 Image Processing Pipeline을 구현하고, 검사 결과의 시각화 및 정량적 성능 평가 기능을 구성했습니다.

**주요 구현 기능**
- ROI 기반 Image Preprocessing
- Gaussian Blur, Top-Hat, Otsu Thresholding을 활용한 결함 후보 검출
- Contour 및 Feature Filtering을 통한 결함 판별
- `DefectInspector` Class 기반 모듈화된 검사 구조
- Batch Processing 및 OK/NG 자동 판정
- Bounding Box 시각화 및 CSV Logging
- Confusion Matrix 기반 정량적 성능 평가

**Tech Stack:** C++20, OpenCV, CMake, Linux

[프로젝트 상세 보기](https://github.com/daeheon1996/final-portfolio/tree/main/industrial-defect-inspection)

---

## Development Environment

| Category | Technologies |
|---|---|
| Languages | C, C++20 |
| Platform | Linux (Ubuntu) |
| Libraries & APIs | POSIX API, POSIX Threads, OpenCV |
| Build Tools | GCC, G++, Make, CMake |
| Debugging & Testing | GDB, Sanitizers, Wireshark |
| Version Control | Git, GitHub |

## Repository Structure

```text
final-portfolio/
├── mini-shell/
├── multithreaded-http-server/
├── industrial-defect-inspection/
└── README.md
```

각 프로젝트의 상세 구현 내용, 실행 방법 및 테스트 결과는 해당 프로젝트 디렉터리의 README에서 확인할 수 있습니다.
