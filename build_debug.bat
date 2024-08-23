@echo off

cls

REM Configure a debug build
cmake -S . -B build-debug/ -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug

cd build-debug

REM Actually build the binaries
ninja -j8 -d explain

cd ..

pause