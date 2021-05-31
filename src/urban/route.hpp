#ifndef __ROUTE_HPP__
#define __ROUTE_HPP__

#include <vector>
#include <osm_net.hpp>

namespace urban {

	class edge_info {
		int _origin;
		int _destin;
		float _time;
		float _dist;

		public:
		edge_info(
			int origin, 
			int destin,
			float time,
			float dist
		);
		edge_info();
		int get_origin() const;
		int get_destin() const;
		float get_time() const;
		float get_dist() const;
	};

	class route {
		int _route_id;
		std::vector<int> _stop_sequence;
		std::vector<edge_info> _edges;
		float _route_length;
		float _total_time;

		public:
		route(int route_id, std::vector<int>& stop_sequence);
		route();

		int get_route_id() const;
		const std::vector<int>& get_stop_sequence() const;
		const std::vector<edge_info>& get_edge_info() const;
		float get_route_length() const;
		int size() const; 
	};

} // namespace urban

#endif