add_library(AttitudeEstimator STATIC)

target_sources(AttitudeEstimator PRIVATE AttitudeEstimator.cpp)
target_include_directories(AttitudeEstimator PUBLIC . ../kalman ../lib)
