cmake_minimum_required(VERSION 3.16.3)

add_library(transit_problem SHARED STATIC
	tndp.hpp
	tndp.cpp
	tndp_configs.hpp
	tndp_configs.cpp
	tndp_constraints.hpp
	tndp_objectives.hpp
	route_pool.hpp
	route_pool.cpp
	tnfsp.hpp
	tnfsp.cpp
	tnfsp_configs.hpp
	tnfsp_configs.cpp
	tnfsp_constraints.hpp
	tnfsp_objectives.hpp)

target_link_libraries(transit_problem 
	                  INTERFACE genetic 
					  PUBLIC osm_net
					  PUBLIC urban)

target_include_directories(transit_problem 
                           PUBLIC ${PROJECT_SOURCE_DIR}/src/transit_problem)
