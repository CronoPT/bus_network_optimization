#ifndef __LISBON_BUS_HPP__
#define __LISBON_BUS_HPP__

#include "bus_network.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <osm_net.hpp>
#include "route.hpp"
#include "configs.hpp"

namespace urban {

	class bus_network;

	class lisbon_bus {
		static bus_network _instance;
		static bus_network import_lisbon_bus();
		static bool _initialized;

		public:
		static bus_network* instance();
	};

} // namespace urban

#endif