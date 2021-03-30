cmake_minimum_required(VERSION 3.16.3)

add_library(net INTERFACE)
target_sources(net INTERFACE 
	edge.hpp	
	node.hpp
	graph.hpp)

target_include_directories(net INTERFACE ${PROJECT_SOURCE_DIR}/src/net)

print(PROJECT_SOURCE_DIR)