add_library(matrix STATIC)
target_sources(matrix PRIVATE
		Matrix.cpp
		)

target_include_directories(matrix PUBLIC .)
