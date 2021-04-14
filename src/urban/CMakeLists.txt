cmake_minimum_required(VERSION 3.16.3)

add_library(urban SHARED STATIC 
	bus_network.hpp
	bus_network.cpp)

target_include_directories(urban PUBLIC ${PROJECT_SOURCE_DIR}/src/urban)

# target_link_libraries(urban PRIVATE osm_net INTERFACE net)
