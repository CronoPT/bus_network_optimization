#ifndef __METRO_NODE_HPP__
#define __METRO_NODE_HPP__

namespace urban {

	class metro_node {
		int _station_id;
		double _lon;
		double _lat;

		public:
		metro_node(int station_id, double lon, double lat);
		metro_node();

		int get_station_id() const;
		double get_lon() const;
		double get_lat() const;
	};

} // namespace urban

#endif