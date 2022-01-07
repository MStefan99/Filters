add_library(matrix_pc STATIC)
target_sources(matrix_pc PRIVATE
		Matrix.cpp
		vector_io.cpp
		)

target_include_directories(matrix_pc PUBLIC .)
