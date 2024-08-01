# HIT internship projects

### All in Cpp

**Python UI版请见 [https://github.com/xin700/Hit](https://github.com/xin700/Hit)**

**本项目共实现：**

**1. 金刚线度量检测**


**2. 金刚线最小圆检测**


**3.二维码传统算法定位**


目录结构:

```
.
├── CMakeLists.txt
├── Img
├── Include
│   ├── dataMatrixSolver.h
│   ├── fileHandler.h
│   ├── imageHandler.h
│   ├── lineSolver.h
│   └── minCircleSolver.h
├── README.md
├── Src
│   ├── dataMatrixSolver.cpp
│   ├── fileHandler.cpp
│   ├── imageHandler.cpp
│   ├── lineSolver.cpp
│   └── minCircleSolver.cpp
└── main.cpp
```

### Requirements

```
1. c++17 (gnu++17) 及以上
2. CMake
3. OpenCV
4. Eigen3
```

**注意非MacOS系统用户请将CMakeLists中第11行set语句注释**

### Build

```shell
mkdir build
cd build
cmake ..
make -j
```
