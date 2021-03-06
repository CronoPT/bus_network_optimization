cmake_minimum_required(VERSION 3.16.3)

add_library(genetic INTERFACE)
target_sources(genetic INTERFACE 
    solution.hpp
    population.hpp
    constraint.hpp
    cost_function.hpp
    problem.hpp
    algorithm.hpp
    classic_ga.hpp
    nsga.hpp
    logger.hpp
    genetic_configs.hpp)

# set(THREADS_PREFER_PTHREAD_FLAG ON)
# find_package(Threads REQUIRED)
# interface_link_libraries(genetic PRIVATE Threads::Threads)

# set_target_properties(genetic PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(genetic INTERFACE ${PROJECT_SOURCE_DIR}/src/genetic)

# install(TARGETS genetic DESTINATION ${GENETIC_INSTALL_LIB_DIR})
# install(FILES 
#     solution.hpp
#     population.hpp
#     DESTINATION ${GENETIC_INSTALL_INCLUDE_DIR})
