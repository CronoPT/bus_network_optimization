#ifndef __GRID_HPP__
#define __GRID_HPP__

#include <vector>
#include "bus_network.hpp"
#include "metro_network.hpp"
#include "walking_network.hpp"
#include <osm_net.hpp>
#include <unordered_map>
#include <set>
#include <limits>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include "configs.hpp"

namespace urban {

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

	class grid {

		const int _divisions = configs::divisions;

		double _min_lon;
		double _min_lat;
		double _max_lon;
		double _max_lat;
		double _lon_step;
		double _lat_step;

		std::unordered_map<int, std::unordered_map<int, square>> _squares;
		
		public:
		grid();
		grid(
			osm_net::osm_net road,
			bus_network bus,
			metro_network metro
		);

		std::pair<int, int> coordinates_to_squares(double lon, double lat) const;
		bool exists_square(int i, int j) const;
		void print_report() const;
		void generate_geojson() const;
		bool element_in(int element, std::set<int> vec);
		std::unordered_map<int, std::unordered_map<int, square>>& get_squares();
		std::string bus_or_metro(
			int node_id, 
			bus_network bus,
			metro_network metro
		);
		bool needs_penalty_bus(net::edge<bus_edge> edge, std::string prev_itinerary);
		bool needs_penalty_metro(net::edge<metro_edge> edge, std::string prev_itinerary);
		std::pair<std::vector<int>, std::vector<float>> best_path_between(
			std::pair<int, int> origin, 
			std::pair<int, int> destin, 
			bus_network bus,
			metro_network metro,
			walking_network walk
		);
		void best_path_between_all(
			std::pair<int, int> origin, 
			std::unordered_map<int, std::unordered_map<int, square>>& squares, 
			bus_network bus,
			metro_network metro,
			walking_network walk
		);
		void print_progress_bar(int iteration, int total);
		int get_total_squares();
		void predict_all_od_pairs(
			bus_network bus,
			metro_network metro,
			walking_network walk	
		);

	};

} // namespace urban

#endif