#ifndef __WALKING_NODE_HPP__
#define __WALKING_NODE_HPP__

#include <string>

namespace urban {

	class walking_node {
		int _id;
		std::string _type;
		double _lon;
		double _lat;

		public:
		walking_node(int id, std::string type, 
		             double lon, double lat);
		walking_node();

		int get_id() const;
		const std::string& get_type() const;
		double get_lon() const;
		double get_lat() const;
	};

} // namespace urban

#endif