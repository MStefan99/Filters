cmake_minimum_required(VERSION 3.16)
project(filters)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Og")

add_compile_options(-DMATRIX_IO)
add_compile_options(-O0)
add_compile_options(-Weverything -Werror
		-Wno-padded -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-unknown-argument -Wno-unused-function
		-Wno-unused-parameter -Wno-global-constructors -Wno-exit-time-destructors -Wno-unknown-pragmas
		-Wno-unused-variable)


add_subdirectory(lib)
add_subdirectory(kalman)
add_subdirectory(estimators)

add_executable(main main.cpp)
target_link_libraries(main Kalman)
target_link_libraries(main AttitudeEstimator)
