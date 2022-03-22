add_library(AttitudeEstimator STATIC)

target_sources(AttitudeEstimator PRIVATE AttitudeEstimator.cpp)
target_include_directories(AttitudeEstimator PUBLIC .)

target_link_libraries(AttitudeEstimator Kalman)
