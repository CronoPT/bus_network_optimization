#ifndef __BUS_NODE_HPP__
#define __BUS_NODE_HPP__

namespace urban {

	class bus_node {
		int _stop_id;
		double _lon;
		double _lat;

		public:
		bus_node(int stop_id, double lon, double lat);
		bus_node();

		int get_stop_id() const;
		double get_lon() const;
		double get_lat() const;
	};

} // namespace urban

#endif