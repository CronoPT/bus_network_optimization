#ifndef __OSM_NODE_HPP__
#define __OSM_NODE_HPP__

#include <string>

namespace osm_net {

	class osm_node {
		int _osm_id;
		double _lon;
		double _lat;
		std::string _type;

		public:
			osm_node(int id, double lon, 
			         double lat, std::string type);
			osm_node();
			int get_osm_id() const;
			double get_lon() const;
			double get_lat() const;
			const std::string& get_type() const;
	};

} // namespace osm_net

#endif