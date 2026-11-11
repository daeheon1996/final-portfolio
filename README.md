# Software Development Portfolio

C/C++ 기반의 소프트웨어 개발 프로젝트를 정리한 포트폴리오입니다.
Linux 환경에서의 System Programming부터 C++/OpenCV를 활용한 Machine Vision까지, 실제 동작하는 소프트웨어를 직접 설계하고 구현하는 데 중점을 두었습니다.

## Projects

### 1. Mini Shell | C

Linux 환경에서 동작하는 간단한 Unix-like Shell 구현 프로젝트입니다.

**주요 구현 기능**
- `fork()`, `execvp()`를 활용한 프로세스 생성 및 명령어 실행
- Built-in Commands (`cd`, `pwd`, `exit`)
- Redirection (명령어의 표준 입출력을 파일로 변경) `echo hello > output.txt`
- Pipe (왼쪽 명령어의 출력을 오른쪽 명령어의 입력으로 받음) `ls -l | grep .c`

**Tech Stack:** C, Linux, GCC, Make

[프로젝트 상세 보기](https://github.com/daeheon1996/final-portfolio/tree/main/mini-shell)

---

### 2. Multithreaded HTTP Server | C

POSIX Threads와 TCP Socket을 활용하여 여러 client의 request를 동시에 처리하는 HTTP Server 구현 프로젝트입니다.

**주요 구현 기능**
- TCP Socket 기반 Client-Server 통신
- `pthread`를 활용한 Thread-per-Client 구조
- HTTP Request Parsing 및 Response 처리
- Mutex와 Condition Variable을 활용한 Thread Synchronization
- Signal Handling을 활용한 Graceful Shutdown

**Tech Stack:** C, Linux, POSIX Threads, TCP/IP, GCC, Make

[프로젝트 상세 보기](https://github.com/daeheon1996/final-portfolio/tree/main/multithreaded-http-server)

---

### 3. Defect Inspection | C++

C++20과 OpenCV를 활용하여 금속 너트의 표면 scratch 여부를 검사하는 프로젝트입니다.

MVTec AD Metal Nut 데이터셋을 활용하여 기본적인 영상처리 알고리즘 동작 과정을 이해하고 실습하는데 중점을 두었습니다.

**주요 구현 기능**
- 고정 ROI 기반 Gaussian Blur, Top-Hat, Otsu Thresholding을 활용한 결함 후보 검출
- Feature Filtering 및 Bounding Box 시각화
- Batch Processing을 통한 전체 이미지 자동 검사 및 CSV Logging
- Confusion Matrix, Classification Metrics 기반 성능 평가

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
