#ifndef __OSM_EDGE_HPP__
#define __OSM_EDGE_HPP__

#include <vector>
#include <string>

namespace osm_net {

	typedef std::vector<std::pair<double, double>> coord_sequence;

	class osm_edge {
		int _origin_id;
		int _destin_id;
		float _length;
		int _max_speed;
		coord_sequence _geometry;

		public:
			osm_edge(int origin, int destin, float len, 
			         int m_speed, coord_sequence geometry);
			osm_edge(int origin, int destin, float len, 
			         int m_speed);
			osm_edge(int origin, int destin, float len);
			osm_edge();
			int get_origin_id() const;
			int get_destin_id() const;
			float get_length()  const;
			int get_max_speed() const;
			const coord_sequence& get_geometry() const;
			bool has_geometry() const;
	};

} // namespace osm_net

#endif