#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <vector>
#include "bus_network.hpp"
#include "metro_network.hpp"
#include "walking_network.hpp"
#include "single_path_report.hpp"
#include "network_usage.hpp"
#include <osm_net.hpp>
#include <unordered_map>
#include <set>
#include <limits>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "configs.hpp"
#include "trip.hpp"
#include "lisbon_bus.hpp"

namespace urban {

	class bus_network;

	#define WALK  -2
	#define START -1
	#define BUS   -3
	#define METRO -4

	/**
	 * A class to hold information about what
	 * belong to each deivision of a grid.
	*/
	class square {
		int _i;
		int _j;
		std::set<int> _bus;
		std::set<int> _metro;

		public:
		square();
		square(int i, int j);
		void add_bus(int stop_id);
		void add_metro(int station_id);
		const std::set<int>& get_bus() const;
		const std::set<int>& get_metro() const; 
	};

	/**
	 * A class to devide a public transportation
	 * network (road, bus and metro) into a grid
	 * in which it is possible to compute shortest
	 * paths (using bus and metro) between the divisions
	 * made. 
	*/
	class grid {

		static grid* _instance;

		const int _divisions = configs::divisions;

		double _min_lon;
		double _min_lat;
		double _max_lon;
		double _max_lat;
		double _lon_step;
		double _lat_step;

		// The squares that make up the network.
		// _squares[i][j] should give us the square in column i
		// and line j.
		std::unordered_map<int, std::unordered_map<int, square>> _squares;
		std::unordered_map<int, int> _node_to_index;
		std::unordered_map<int, int> _node_modes;
		int _total_nodes;

		// Auxiliary structures to the path finding algorithm.
		// Very similar structures to those in Dijkstra's.
		
		grid();

		public:
		static grid* instance();


		std::pair<int, int> coordinates_to_squares(double lon, double lat) const;
		bool exists_square(int i, int j) const;
		void print_report() const;
		void generate_geojson() const;
		std::unordered_map<int, std::unordered_map<int, square>>& get_squares();
		bool needs_penalty_bus(net::edge<bus_edge>& edge, int prev_itinerary);
		bool needs_penalty_metro(net::edge<metro_edge>& edge, int prev_itinerary);
		std::vector<single_path_report> best_path_between_all(
			std::pair<int, int> origin, 
			bus_network& bus
		);
		void print_progress_bar(int iteration, int total);
		int get_total_squares();
		void print_report(
			std::pair<int, int> origin, 
			std::vector<single_path_report>& report
		);
		network_usage predict_all_od_pairs(
			bus_network& bus
		);
		std::vector<trip> trip_from_report(
			std::pair<int, int> origin,
			std::vector<single_path_report>& report,
			bus_network& bus
		);

		inline static bool is_metro_node(int node);
	};

} // namespace urban

#endif