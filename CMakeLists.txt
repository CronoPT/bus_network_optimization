cmake_minimum_required(VERSION 3.16.3)

project(bus_network_optimization LANGUAGES CXX)

function(print var)
	message("${var} = ${${var}}")
endfunction()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})

set(GENETIC_INSTALL_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(GENETIC_INSTALL_BIN_DIR ${PROJECT_SOURCE_DIR}/bin)
set(GENETIC_INSTALL_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)

set(GENETIC_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/genetic)
set(NET_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/net)
set(OSM_NET_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/osm_net)
set(URBAN_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/urban)
set(TRANSIT_PROBLEM_HEADERS_DIR ${PROJECT_SOURCE_DIR}/src/transit_problem)

include_directories(${GENETIC_INSTALL_INCLUDE_DIR})
include_directories(${GENETIC_HEADERS_DIR})
include_directories(${NET_HEADERS_DIR})
include_directories(${OSM_NET_HEADERS_DIR})
include_directories(${URBAN_HEADERS_DIR})
include_directories(${TRANSIT_PROBLEM_HEADERS_DIR})

add_subdirectory(src)
add_subdirectory(tests)
