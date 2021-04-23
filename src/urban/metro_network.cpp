#ifndef __METRO_NETWORK_CPP__
#define __METRO_NETWORK_CPP__

#include "metro_network.hpp"

namespace urban {

	metro_network::metro_network(): graph() {
		std::ifstream input_file(configs::metro_network);
		nlohmann::json json_net = nlohmann::json::parse(input_file);

		for (auto& node: json_net["nodes"]) {
			add_node(node["id"], metro_node(
				node["id"],
				node["x"],
				node["y"]
			));
		}

		for (auto& adja: json_net["adjacency"]) {
			for (auto& edge: adja) {
				add_edge(edge["origin_id"], edge["destin_id"], metro_edge(
					edge["origin_id"],
					edge["destin_id"],
					edge["line_color"],
					edge["time"]
				));
			}
		}
	}

} // namespace urban

#endif