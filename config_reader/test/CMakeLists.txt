cmake_minimum_required(VERSION 3.5)

project(config_reader_test LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#file(GLOB_RECURSE DATA resources/*)
#file(COPY ${DATA} DESTINATION resources)

add_executable(
	${PROJECT_NAME}
	config_reader_test.cpp
#	${DATA}
)

#find_package(Catch2 REQUIRED)
#target_link_libraries(${PROJECT_NAME} config_parser Catch2::Catch2)

find_package(GTest REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE config_reader GTest::GTest GTest::Main)
#gtest_discover_tests(${PROJECT_NAME})
gtest_add_tests(
	TARGET ${PROJECT_NAME}
	TEST_LIST test_list
)
set_tests_properties(${test_list} PROPERTIES
					 ENVIRONMENT
					 DATADIR=${CMAKE_CURRENT_SOURCE_DIR}/resources)






#add_test(test1 ${PROJECT_NAME} 1)
#add_test(file_test file_test "${CMAKE_CURRENT_SOURCE_DIR}/resources")


