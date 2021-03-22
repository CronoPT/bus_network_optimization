cmake_minimum_required(VERSION 3.16.3)

add_library(genetic SHARED STATIC 
    solution.hpp
    solution.cpp)

install(TARGETS genetic DESTINATION ${GENETIC_INSTALL_LIB_DIR})
install(FILES solution.hpp DESTINATION ${GENETIC_INSTALL_INCLUDE_DIR})
