cmake_minimum_required(VERSION 3.16.3)

include_directories(${GENETIC_HEADERS_DIR})
find_package(GTest REQUIRED)

add_executable(main_tests genetic/solution_test.cpp)
target_link_libraries(main_tests genetic GTest::Main)
install(TARGETS main_tests DESTINATION bin)
