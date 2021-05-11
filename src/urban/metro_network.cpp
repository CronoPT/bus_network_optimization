#ifndef __METRO_NETWORK_CPP__
#define __METRO_NETWORK_CPP__

#include "metro_network.hpp"

namespace urban {

	metro_network::metro_network(): graph() {
		std::ifstream input_file(configs::metro_network);
		nlohmann::json json_net = nlohmann::json::parse(input_file);
		auto lines = std::unordered_map<std::string, int>();
		int metro_ids = 99990;

		for (auto& node: json_net["nodes"]) {
			add_node(node["id"], metro_node(
				node["id"],
				node["x"],
				node["y"]
			));
		}

		for (auto& adja: json_net["adjacency"]) {
			for (auto& edge: adja) {

				if (lines.find(edge["line_color"]) == lines.end()) {
					lines[edge["line_color"]] = metro_ids++;
				}

				add_edge(edge["origin_id"], edge["destin_id"], metro_edge(
					edge["origin_id"],
					edge["destin_id"],
					lines[edge["line_color"]],
					edge["time"]
				));
			}
		}
	}

	metro_network* metro_network::instance() {
		if (!_instance) {
			_instance = new metro_network();
		}
		return _instance;
	}

	metro_network* metro_network::_instance = nullptr;

} // namespace urban

#endif