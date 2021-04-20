#ifndef __PREDICTOR_HPP__
#define __PREDICTOR_HPP__

#include <vector>
#include "bus_network.hpp"
#include "metro_network.hpp"
#include <osm_net.hpp>
#include <unordered_map>
#include <set>
#include <limits>

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
	};

	class grid {

		const int _divisions = 50;

		double _min_lon;
		double _min_lat;
		double _max_lon;
		double _max_lat;
		double _lon_step;
		double _lat_step;

		std::unordered_map<int, std::unordered_map<int, square>> _squares;
		
		public:
		grid(
			osm_net::osm_net road,
			bus_network bus,
			metro_network metro
		);

		std::pair<int, int> coordinates_to_squares(double lon, double lat);
		bool exists_square(int i, int j);

	};

} // namespace urban

#endif