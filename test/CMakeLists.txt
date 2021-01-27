cmake_minimum_required(VERSION 3.5)

project(parser_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
	${PROJECT_NAME}
	parser_test.cpp
)

find_package(GTest REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE config_parser GTest::GTest GTest::Main)
gtest_add_tests(
	TARGET ${PROJECT_NAME}
	TEST_LIST test_list
)
set_tests_properties(${test_list} PROPERTIES
					 ENVIRONMENT
					 DATADIR=${CMAKE_CURRENT_SOURCE_DIR}/resources)
