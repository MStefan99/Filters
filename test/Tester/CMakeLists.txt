# Creating Tester library
add_library(Tester)
set(TESTER_DIRECTORY .)
set(TESTER_SOURCE_DIRECTORY ${TESTER_DIRECTORY}/src)
target_include_directories(Tester PUBLIC ${TESTER_DIRECTORY}/inc)


target_sources(Tester PRIVATE
		${TESTER_SOURCE_DIRECTORY}/AssertException.cpp
		${TESTER_SOURCE_DIRECTORY}/StatRecorder.cpp
		${TESTER_SOURCE_DIRECTORY}/Test.cpp
		${TESTER_SOURCE_DIRECTORY}/TestSuite.cpp
		${TESTER_SOURCE_DIRECTORY}/Tester.cpp
		)
