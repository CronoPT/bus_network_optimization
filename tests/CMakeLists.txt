cmake_minimum_required(VERSION 3.16.3)

find_package(GTest REQUIRED)

add_executable(main_tests 
	genetic/solution_test.cpp
	genetic/population_test.cpp
	net/graph_test.cpp
	net/priority_queue_test.cpp
	urban/exists_square_test.cpp)
	
target_link_libraries(main_tests INTERFACE genetic 
                                 INTERFACE net
								 PUBLIC osm_net 
								 PUBLIC urban
                                 PUBLIC GTest::Main
								 )

install(TARGETS main_tests DESTINATION bin)
