add_library(Kalman STATIC)
target_sources(Kalman PRIVATE Kalman.cpp)
target_include_directories(Kalman PUBLIC .)
target_link_libraries(Kalman matrix_pc)

add_executable(main main.cpp)
target_link_libraries(main Kalman)
