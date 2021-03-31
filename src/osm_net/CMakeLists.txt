cmake_minimum_required(VERSION 3.16.3)

find_package(nlohmann_json 3.2.0 REQUIRED)

add_library(osm_net SHARED STATIC 
	osm_net.hpp
	osm_net.cpp
	osm_edge.hpp
	osm_node.hpp)

target_include_directories(osm_net PUBLIC ${PROJECT_SOURCE_DIR}/src/osm_net)

target_link_libraries(osm_net PRIVATE nlohmann_json::nlohmann_json)

# install(TARGETS genetic DESTINATION ${GENETIC_INSTALL_LIB_DIR})
# install(FILES 
#     solution.hpp
#     population.hpp
#     DESTINATION ${GENETIC_INSTALL_INCLUDE_DIR})
