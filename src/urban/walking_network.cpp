#ifndef __WALKING_NETWORK_CPP__
#define __WALKING_NETWORK_CPP__

#include "walking_network.hpp"

namespace urban {

	walking_network::walking_network(): graph() {
		std::ifstream input_file(configs::walking_network);
		nlohmann::json json_walks = nlohmann::json::parse(input_file);

		for (auto& walk_item: json_walks.items()) {
			int id = std::stoi(walk_item.key());
			auto& type  = walk_item.value()["type"];
			auto& point = walk_item.value()["point"];
			add_node(id, walking_node(
				id, type, point[0], point[1]
			));
		}

		for (auto& walk_item: json_walks.items()) {
			int   origin_id   = std::stoi(walk_item.key());
			auto& origin_type = walk_item.value()["type"];

			for (auto& bus: walk_item.value()["close_bus"]) {
				int destin_id    = bus["stop_id"]; 
				float time_taken = bus["time"];
				add_edge(origin_id, destin_id, walking_edge(
					origin_id, destin_id, time_taken,
					origin_type, "bus"
				));
			}

			for (auto& metro: walk_item.value()["close_metro"]) {
				int destin_id    = metro["station_id"]; 
				float time_taken = metro["time"];
				add_edge(origin_id, destin_id, walking_edge(
					origin_id, destin_id, time_taken,
					origin_type, "metro"
				));
			}

		}

	}

} // namespace urban

#endif