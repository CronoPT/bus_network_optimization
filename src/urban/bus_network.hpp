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
#include "grid.hpp"

namespace urban {

	class bus_network: public net::graph<bus_node, bus_edge> {
		
		bool  _evaluated;
		float   _transfers;
		float _in_vehicle_time;
		float _total_length;
		float _unsatisfied_demand;
		int   _number_routes;
		int   _shortest_route;
		int   _longest_route;
		std::vector<route> _routes;

		public:
		bus_network(std::vector<route>& routes);
		bus_network();

		bool evaluated() const;
		float get_transfers() const;
		float get_in_vehicle_time() const;
		float get_total_length() const;
		float get_unsatisfied_demand() const;
		int  get_number_routes() const;
		int  get_shortest_route() const;
		int  get_longest_route() const;
		const std::vector<route> get_routes() const;

		void evaluate();

		friend std::ostream& operator<<(std::ostream& os, const bus_network& a); 

	};

} // namespace urban

#endif