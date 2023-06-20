add_library(FixMath STATIC)
target_sources(FixMath PRIVATE
		FixMath.cpp
		)

target_include_directories(FixMath PUBLIC .)
