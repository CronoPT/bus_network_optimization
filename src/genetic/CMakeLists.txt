cmake_minimum_required(VERSION 3.16.3)

add_library(genetic INTERFACE)
target_sources(genetic INTERFACE 
    solution.hpp
    population.hpp
    constraint.hpp
    cost_function.hpp
    problem.hpp)

# set_target_properties(genetic PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(genetic INTERFACE .)

# install(TARGETS genetic DESTINATION ${GENETIC_INSTALL_LIB_DIR})
# install(FILES 
#     solution.hpp
#     population.hpp
#     DESTINATION ${GENETIC_INSTALL_INCLUDE_DIR})
