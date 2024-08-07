cmake_minimum_required(VERSION 3.0.2)
project(waz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Torch_DIR "/opt/libtorch/share/cmake/Torch")
set(TORCH_ROOT "/opt/libtorch")
#set(Torch_INCLUDE_DIRS "/opt/libtorch/include")

set(Json_INCLUDE_DIRS "/usr/include/jsoncpp")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(BUILD_ROS "Build ROS" ON)

find_package(catkin REQUIRED COMPONENTS
	roscpp
	nav_msgs
	visualization_msgs
	sensor_msgs
	std_msgs
	cv_bridge
	image_transport
	tf2_ros
)

find_package(Torch REQUIRED PATHS "${TORCH_ROOT}")
find_package(TBB REQUIRED)
find_package(OpenCV REQUIRED)
find_package(glog 0.4.0 REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}
    ${TORCH_INCLUDE_DIRS}
    ${Json_INCLUDE_DIRS}
    ${glog_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIRS}
    ${EIGEN3_INCLUDE_DIR}
    ${TBB_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME}
    src/network_manager.cpp
    src/depth.cpp
    src/semantics.cpp
    src/cost_map.cpp
    src/waz.cpp
    src/astar.cpp
    src/velocity.cpp
    utils/label_map.cpp
    utils/params.cpp
    utils/resize.cpp
    utils/normalize.cpp
    utils/interpolate.cpp
    utils/average.cpp
    utils/mode.cpp
    utils/max.cpp
)

add_executable(wazros ros/wazros.cpp ros/wazros_node.cpp)
target_link_libraries(wazros
    ${PROJECT_NAME}
    ${TORCH_LIBRARIES}
    ${OpenCV_LIBRARIES}
    jsoncpp_lib
    ${glog_LIBRARIES}
    ${GTEST_LIBRARIES}
    TBB::tbb
    pthread
    Eigen3::Eigen
    ${catkin_LIBRARIES}
    )

add_executable(cost-map-test test/cpp/cost_map_test.cpp)
target_link_libraries(cost-map-test
    ${PROJECT_NAME}
    ${TORCH_LIBRARIES}
    ${OpenCV_LIBRARIES}
    jsoncpp_lib
    ${glog_LIBRARIES}
    ${GTEST_LIBRARIES}
    TBB::tbb
    pthread
    Eigen3::Eigen
    )

add_executable(waz-test test/cpp/waz_test.cpp)
target_link_libraries(waz-test
    ${PROJECT_NAME}
    ${TORCH_LIBRARIES}
    ${OpenCV_LIBRARIES}
    jsoncpp_lib
    ${glog_LIBRARIES}
    ${GTEST_LIBRARIES}
    TBB::tbb
    pthread
    Eigen3::Eigen
    )

add_executable(preprocess-test test/cpp/preprocess.cpp)
target_link_libraries(preprocess-test
    ${PROJECT_NAME}
    ${TORCH_LIBRARIES}
    ${OpenCV_LIBRARIES}
    jsoncpp_lib
    ${glog_LIBRARIES}
    ${GTEST_LIBRARIES}
    pthread
    Eigen3::Eigen
    )
