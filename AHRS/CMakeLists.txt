add_library(AHRS STATIC)

target_sources(AHRS PRIVATE AHRS.cpp)
target_include_directories(AHRS PUBLIC .)

target_link_libraries(AHRS Kalman)
