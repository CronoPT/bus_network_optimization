#ifndef __BUS_NETWORK_HPP__
#define __BUS_NETWORK_HPP__

#include "bus_node.hpp"
#include "bus_edge.hpp"
#include "metro_network.hpp"
#include "walking_network.hpp"
#include "odx_matrix.hpp"
#include "route.hpp"
#include "network_usage.hpp"
#include <graph.hpp>
#include <vector>
#include <osm_net.hpp>
#include <fstream>
#include <string>
#include <limits>
#include "configs.hpp"

namespace urban {

	class bus_network: public net::graph<bus_node, bus_edge> {
		
		bool  _evaluated;
		int   _transfers;
		float _in_vehicle_time;
		float _total_length;
		float _unsatisfied_demand;
		int   _number_routes;
		int   _shortest_route;
		int   _longest_route;

		public:
		bus_network(
			std::vector<route>& routes,
			osm_net::osm_net& road_network
		);

		bool evaluated() const;
		int  get_transfers() const;
		float get_in_vehicle_time() const;
		float get_total_length() const;
		float get_unsatisfied_demand() const;
		int  get_number_routes() const;
		int  get_shortest_route() const;
		int  get_longest_route() const;

		void evaluate(
			network_usage& usage, 
			metro_network& metro, 
			walking_network& walk,
			odx_matrix& odx
		);

	};

} // namespace urban

#endif