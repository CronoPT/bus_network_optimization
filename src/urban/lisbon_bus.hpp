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

	bus_network import_lisbon_bus(osm_net::osm_net graph);

} // namespace urban

#endif