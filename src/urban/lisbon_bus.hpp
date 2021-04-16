#ifndef __LISBON_BUS_HPP__
#define __LISBON_BUS_HPP__

#include "bus_network.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>

namespace urban {

	bus_network import_lisbon_bus(road_graph graph);

} // namespace urban

#endif