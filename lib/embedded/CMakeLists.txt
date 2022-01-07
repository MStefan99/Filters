add_library(matrix_embedded STATIC)
target_sources(matrix_embedded PRIVATE
		Matrix.cpp
		)

target_include_directories(matrix_embedded PUBLIC .)
