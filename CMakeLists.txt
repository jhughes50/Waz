cmake_minimum_required(VERSION 3.0.2)
project(waz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Torch_DIR "/opt/libtorch/share/cmake/Torch")
set(Torch_INCLUDE_DIRS "/opt/libtorch/include")

set(Json_INCLUDE_DIRS "/usr/include/jsoncpp")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Torch REQUIRED)
find_package(TBB REQUIRED)
find_package(OpenCV REQUIRED)
find_package(glog 0.4.0 REQUIRED)
find_package(jsoncpp REQUIRED)

include_directories(
    include
    ${OpenCV_INCLUDE_DIRS}
    ${Torch_INCLUDE_DIRS}
    ${Json_INCLUDE_DIRS}
    ${glog_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
    src/network_manager.cpp
    src/params.cpp
)

add_executable(model-test test/models.cpp)
target_link_libraries(model-test
    ${PROJECT_NAME}
    ${TORCH_LIBRARIES}
    ${OpenCV_LIBRARIES}
    jsoncpp_lib
    ${glog_LIBRARIES}
    )
set_property(TARGET model-test PROPERTY CXX_STANDARD 17)
