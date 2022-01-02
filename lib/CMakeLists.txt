add_library(matrix STATIC)
target_sources(matrix PRIVATE
		Matrix.cpp
		vector_io.cpp
		)

target_include_directories(matrix PUBLIC .)
