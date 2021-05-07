cmake_minimum_required(VERSION 3.16.3)

add_library(transit_problem SHARED STATIC
	tndp.hpp
	tndp.cpp)

target_link_libraries(transit_problem 
	                  INTERFACE genetic 
					  PUBLIC osm_net
					  PUBLIC urban)

target_include_directories(transit_problem 
                           PUBLIC ${PROJECT_SOURCE_DIR}/src/transit_problem)
