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
		bool  _statics_computed;
		float   _transfers;
		float _in_vehicle_time;
		float _total_length;
		float _unsatisfied_demand;
		int   _number_routes;
		int   _shortest_route;
		int   _longest_route;
		std::vector<route> _routes;
		std::set<int> _route_check;
		std::unordered_map<int, std::set<int>> _stop_in_routes;

		static bool _stops_loaded;
		static std::unordered_map<int, std::pair<double, double>> _bus_stops;

		public:
		bus_network(std::vector<route>& routes);
		bus_network();

		void embed_route(route& r);
		void embed_node(int stop_id, int route_id);

		bool evaluated() const;
		float get_transfers() ;
		float get_in_vehicle_time();
		float get_total_length();
		float get_unsatisfied_demand();
		int  get_number_routes() const;
		int  get_shortest_route();
		int  get_longest_route();
		const std::vector<route>& get_routes() const;

		void add_route(route& new_route);
		void delete_route(int position);
		void delete_node(int stop_id, int route_id);
		bool has_route(int route_id);

		void static_computes();
		void evaluate();

		static int node_id(int stop_id, int route_id);
		static std::pair<int, int> node_id(int node_id);

		friend std::ostream& operator<<(std::ostream& os, const bus_network& a); 

	};

} // namespace urban

#endif