cmake_minimum_required(VERSION 3.16.3)

add_subdirectory(genetic)
add_subdirectory(net)
add_subdirectory(osm_net)
add_subdirectory(urban)
add_subdirectory(transit_problem)

add_executable(main main.cpp)
add_executable(main_x main_x.cpp)
add_executable(main_ciara main_ciara.cpp)
add_executable(urban_main urban_main.cpp)

target_link_libraries(main INTERFACE genetic INTERFACE net PUBLIC osm_net)
target_link_libraries(main_x INTERFACE genetic INTERFACE net PUBLIC osm_net)
target_link_libraries(main_ciara INTERFACE genetic INTERFACE net PUBLIC osm_net)
target_link_libraries(urban_main INTERFACE genetic 
                                 INTERFACE net 
								 PUBLIC osm_net 
								 PUBLIC urban
								 PUBLIC transit_problem)

install(TARGETS main DESTINATION ${GENETIC_INSTALL_BIN_DIR})
install(TARGETS main_x DESTINATION ${GENETIC_INSTALL_BIN_DIR})
install(TARGETS main_ciara DESTINATION ${GENETIC_INSTALL_BIN_DIR})
install(TARGETS urban_main DESTINATION ${GENETIC_INSTALL_BIN_DIR})
