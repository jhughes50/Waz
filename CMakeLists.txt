cmake_minimum_required(VERSION 3.0.2)
project(waz)

add_compile_options(-std=c++11)

find_package(Torch REQUIRED)
find_package(TBB REQUIRED)
find_package(OpenCV REQUIRED)
find_package(glog REQUIRED)
