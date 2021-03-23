cmake_minimum_required(VERSION 3.16.3)

add_subdirectory(genetic)

add_executable(main main.cpp)
target_link_libraries(main INTERFACE genetic)

install(TARGETS main DESTINATION ${GENETIC_INSTALL_BIN_DIR})
