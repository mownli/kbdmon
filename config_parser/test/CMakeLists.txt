cmake_minimum_required(VERSION 3.5)

project(config_parser_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(GTest)
#include(GoogleTest)
#gtest_add_tests(
#	TARGET ${PROJECT_NAME}
#	TEST_SUFFIX .noArgs
#	#TEST_LIST   noArgsTests
#)



#file(GLOB_RECURSE DATA resources/*)
#file(COPY ${DATA} DESTINATION resources)

add_executable(${PROJECT_NAME}
	config_parser_test.cpp
#	${DATA}
)

add_executable(file_test
	file_test.cpp
)


target_link_libraries(${PROJECT_NAME} config_parser)
#target_link_libraries(${PROJECT_NAME} PUBLIC config_parser GTest::gtest GTest::gtest_main)

enable_testing()
add_test(test1 ${PROJECT_NAME} 1)

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}/resources")

add_test(file_test file_test "${CMAKE_CURRENT_SOURCE_DIR}/resources")


