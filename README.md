# rp2040 freertos template


# Building
* Install RP2040 ARM toolkit and Pico SDK, set PICO_SDK_PATH in root CMakeLists.txt
* Clone the repo and install all submodules with `git submodule update --init --recursive`
* If using IDE like CLion: Open root CMakeLists.txt file with CLion and CMake project will automatically be created
* If not using IDE: Manually create `mkdir cmake-build-debug` directory in project root
* Build RP2040 binary: `cd cmake-build-debug` and run `cmake ..`
