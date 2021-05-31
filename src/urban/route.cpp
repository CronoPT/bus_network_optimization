#ifndef __ROUTE_CPP__
#define __ROUTE_CPP__

#include "route.hpp"

namespace urban {

	float compute_path_time_v2(
		std::pair<std::vector<int>, float>& path_report
	) {
		float time  = 0; 
		auto& path = path_report.first; 
		int origin = -1;
		for (auto destin: path) {
			if (origin != -1) {
				auto& adjs = osm_net::osm_net::instance()->get_nodes()[origin].get_adjacencies();

				float min_cost = std::numeric_limits<float>::max();
				int min_key = -1;
				for (auto& adj: adjs) {
					if (adj.second.get_destin() == destin) {
						if (adj.second.get_attributes().get_length() < min_cost) {
							min_cost = adj.second.get_attributes().get_length();
							min_key  = adj.first;
						}
					}
				}
				float length = adjs[min_key].get_attributes().get_length();
				float speed  = adjs[min_key].get_attributes().get_max_speed();
				time += (length / (speed / 3.6));
			}	
			origin = destin;
		}
		return time;
	}



	edge_info::edge_info(
		int origin, 
		int destin,
		float time,
		float dist
	): _origin(origin), _destin(destin),
	 _time(time), _dist(dist) {
		/* Do Nothing */
	}

	edge_info::edge_info(): 
	 _origin(-1), _destin(-1),
	 _time(-1), _dist(-1) {
		/* Do Nothing */
	}

	int edge_info::get_origin() const {
		return _origin;
	}

	int edge_info::get_destin() const {
		return _destin;
	}

	float edge_info::get_time() const {
		return _time;
	}

	float edge_info::get_dist() const {
		return _dist;
	}

	route::route(int route_id, std::vector<int>& stop_sequence):
	 _route_id(route_id), _stop_sequence(stop_sequence),
	 _edges(), _route_length(0) {
		
		int origin_stop = -1;
		for (auto destin_stop: stop_sequence) {
			if (origin_stop != -1) {
				auto path_report = osm_net::osm_net::instance()->dijkstra(
					origin_stop,
					destin_stop,
					[](net::edge<osm_net::osm_edge>& e) -> float {
						auto length = e.get_attributes().get_length();
						auto speed  = e.get_attributes().get_max_speed();
						return (float) length;
					}
				);

				_route_length += path_report.second;
				auto travel_time = compute_path_time_v2(path_report);
				_total_time   += travel_time;
				_edges.push_back(edge_info(
					origin_stop,
					destin_stop,
					travel_time,
					path_report.second
				));
			}
			origin_stop = destin_stop;
		}

	}

	route::route():
	 _route_id(-1), _stop_sequence(), 
	 _edges(), _route_length(-1) {
		/* Do Nothing */
	}

	int route::get_route_id() const {
		return _route_id;
	}

	const std::vector<int>& route::get_stop_sequence() const {
		return _stop_sequence;
	}

	const std::vector<edge_info>& route::get_edge_info() const {
		return _edges;
	}
	
	float route::get_route_length() const {
		return _route_length;
	}

	int route::size() const { return _stop_sequence.size(); }

} // namespace urban

#endif