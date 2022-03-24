cmake_minimum_required(VERSION 3.16)
project(filters)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")

add_subdirectory(estimators)


add_executable(main main.cpp)
target_link_libraries(main AttitudeEstimator)
