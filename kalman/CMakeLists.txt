add_library(Kalman STATIC)

target_sources(Kalman PRIVATE EKF.cpp Kalman.cpp)
target_include_directories(Kalman PUBLIC .)

target_link_libraries(Kalman matrix)
