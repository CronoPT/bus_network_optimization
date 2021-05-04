cmake_minimum_required(VERSION 3.16.3)

add_library(urban SHARED STATIC
	bus_edge.hpp
	bus_edge.cpp
	bus_network.hpp
	bus_network.cpp
	bus_node.hpp
	bus_node.cpp
	lisbon_bus.hpp
	lisbon_bus.cpp
	metro_edge.hpp
	metro_edge.cpp
	metro_network.hpp
	metro_network.cpp
	metro_node.hpp
	metro_node.cpp
	grid.hpp
	grid.cpp
	route.hpp
	route.cpp
	walking_edge.hpp
	walking_edge.cpp
	walking_network.hpp
	walking_network.cpp
	walking_node.hpp
	walking_node.cpp
	configs.hpp
	configs.cpp
	trip.hpp
	trip.cpp
	stage.hpp
	stage.cpp
	single_path_report.hpp
	single_path_report.cpp)

target_link_libraries(urban INTERFACE net PUBLIC osm_net)

target_include_directories(urban PUBLIC ${PROJECT_SOURCE_DIR}/src/urban)

# target_link_libraries(urban PRIVATE osm_net INTERFACE net)
