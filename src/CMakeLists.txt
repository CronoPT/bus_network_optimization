cmake_minimum_required(VERSION 3.16.3)

add_subdirectory(genetic)
add_subdirectory(net)
add_subdirectory(osm_net)

add_executable(main main.cpp)
target_link_libraries(main INTERFACE genetic INTERFACE net PUBLIC osm_net)

install(TARGETS main DESTINATION ${GENETIC_INSTALL_BIN_DIR})
