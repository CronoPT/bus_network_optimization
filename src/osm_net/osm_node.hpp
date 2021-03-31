#ifndef __OSM_NODE_HPP__
#define __OSM_NODE_HPP__

namespace osm_net {

	class osm_node {
		int _osm_id;
		double _lon;
		double _lat;

		public:
			osm_node(int id, double lon, double lat);
			osm_node();
			int get_osm_id() const;
			double get_lon() const;
			double get_lat() const;
	};

} // namespace osm_net

#endif