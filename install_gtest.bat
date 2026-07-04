@echo off
chcp 65001 >nul
echo === Installing Google Test ===

if exist gtest_lib\googletest\include\gtest\gtest.h (
    echo Google Test headers found
    if exist gtest_lib\build\lib\libgtest.a (
        echo === Google Test is ready ===
        exit /b
    ) else (
        echo Libraries not found, rebuilding...
        cd gtest_lib
        if exist build rmdir /s /q build
        mkdir build
        cd build
        set CC=gcc
        set CXX=g++
        C:\msys64\ucrt64\bin\cmake.exe .. -G "MinGW Makefiles" -DBUILD_GMOCK=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
        C:\msys64\ucrt64\bin\mingw32-make.exe
        cd ..\..
        echo === Google Test rebuilt ===
        exit /b
    )
)

echo Cloning Google Test...
git clone https://github.com/google/googletest.git gtest_lib
cd gtest_lib
mkdir build
cd build
set CC=gcc
set CXX=g++
C:\msys64\ucrt64\bin\cmake.exe .. -G "MinGW Makefiles" -DBUILD_GMOCK=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
C:\msys64\ucrt64\bin\mingw32-make.exe
cd ..\..
echo === Google Test installed ===