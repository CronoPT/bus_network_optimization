
#include "osm_net.hpp"

namespace osm_net {

	net::graph<osm_node, osm_edge> import_network() {

		std::ifstream input_file("../data/final_network.json");
		nlohmann::json json_net = nlohmann::json::parse(input_file);

		auto network  = net::graph<osm_node, osm_edge>();
		auto node_ids = std::vector<int>();
		for (auto& json_node: json_net["nodes"]) {
			int id = json_node["id"]; 	
			double lon = json_node["x"];
			double lat = json_node["y"];
			network.add_node(id, osm_node(id, lon, lat));
			node_ids.push_back(id);
		}

		for (int i=0; i<node_ids.size(); i++) {
			auto origin_id = node_ids.at(i);
			auto& adjancecies = json_net["adjacency"].at(i);

			for (auto& adja: adjancecies) {
				int destin_id = adja["id"];
				int max_speed = -1;
				if (adja.contains("maxspeed")) {
					if (adja["maxspeed"].is_number()) {
						max_speed = std::stoi((std::string)adja["maxspeed"]);
					} else {
						for (auto num: adja["maxspeed"]) { 
							max_speed += std::stoi((std::string)num);
						}
						max_speed /= adja["maxspeed"].size();
					}
				}
				
				float length  = adja["length"];
				if (adja.contains("geometry")) {
					auto geo = coord_sequence();
					for (auto& coords: adja["geometry"]) {
						geo.push_back(std::pair<double, double>(
							coords.at(0), coords.at(1)
						));
					}
					network.add_edge(origin_id, destin_id, osm_edge(
						origin_id, destin_id, length, max_speed, geo 
					));
				} else {
					network.add_edge(origin_id, destin_id, osm_edge(
						origin_id, destin_id, length, max_speed 
					));
				}
			}
		}

		return network;
	}

} // namespace osm_net