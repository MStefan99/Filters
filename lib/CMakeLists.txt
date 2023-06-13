add_library(matrix STATIC)
target_sources(matrix PRIVATE
		Matrix.cpp
		)

add_library(FixMath STATIC)
target_sources(FixMath PRIVATE
		FixMath.cpp
		)

target_include_directories(matrix PUBLIC .)
target_include_directories(FixMath PUBLIC .)
