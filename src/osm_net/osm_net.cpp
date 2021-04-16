
#include "osm_net.hpp"

namespace osm_net {

	osm_net::osm_net() {
		std::ifstream input_file("../data/json/network_with_metro.json");
		nlohmann::json json_net = nlohmann::json::parse(input_file);

		auto node_ids = std::vector<int>();
		for (auto& json_node: json_net["nodes"]) {
			int id = json_node["id"]; 	
			double lon = json_node["x"];
			double lat = json_node["y"];

			if (json_node.contains("type")) {
				std::string type = json_node["type"];
				add_node(id, osm_node(id, lon, lat, type));
			} else {
				add_node(id, osm_node(id, lon, lat, "road"));
			}

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
					add_edge(origin_id, destin_id, osm_edge(
						origin_id, destin_id, length, max_speed, geo 
					));
				} else {
					add_edge(origin_id, destin_id, osm_edge(
						origin_id, destin_id, length, max_speed 
					));
				}
			}
		}
	}

	// std::pair<std::vector<int>, float> osm_net::moded_dijkstra(
	// 	int origin_id, 
	// 	int destin_id,
	// 	float (*weight)(net::edge<osm_edge>&),
	// 	std::string type
	// ) {
	// 	const float metro_speed = 60;
	// 	const float transfer_penalty = 60*10; // transfer penalty in seconds
 
	// 	auto queue = net::priority_queue<int>();
	// 	auto dist  = std::unordered_map<int, float>();
	// 	auto prev  = std::unordered_map<int, int>();
		
	// 	const int undefined = -1;

	// 	for (auto& v_pair: _graph.get_nodes()) {
	// 		auto id    = v_pair.first;
	// 		auto& node = v_pair.second; 
	// 		if (node.is_of_type(type)) {
	// 			dist[id] = std::numeric_limits<float>::infinity();
	// 			prev[id] = undefined;
	// 			queue.push(id, dist[id]);
	// 		}
	// 	}
	// 	dist[origin_id] = 0.0;
	// 	queue.update(origin_id, 0.0);

	// 	while (!queue.empty()) {

	// 		auto u = queue.pop();

	// 		if (u == destin_id) { break; }

	// 		auto& u_node = _graph.get_nodes()[u];
	// 		for (auto& edge_info: u_node.get_adjacencies()) {
	// 			auto   key  = edge_info.first;
	// 			auto&  edge = edge_info.second;
	// 			auto   v    = edge.get_destin(); 
				
	// 			auto max_speed = edge.get_attributes().get_max_speed();
	// 			auto& v_node   = _graph.get_nodes()[v];
	// 			if (v_node.is_of_type(type)) {
	// 				if (queue.contains(v)) {

	// 					float alt = dist[u];
	// 					if (v_node.get_attributes().get_type() ==
	// 					    u_node.get_attributes().get_type()) {
	// 						alt += weight(edge)*3.6*max_speed;
	// 					} else {
	// 						alt += weight(edge)*3.6*max_speed + transfer_penalty;
	// 					}

	// 					if (alt < dist[v]) {
	// 						dist[v] = alt;
	// 						prev[v] = u;
	// 						queue.update(v, dist[v]);
	// 					}
	// 				}
	// 			}
	// 		}

	// 	}

	// 	float cost = dist[destin_id];
	// 	auto stk   = std::stack<int>();
	// 	int  u = destin_id;
	// 	if (prev[u]!=undefined || u==origin_id) {
	// 		while (u != undefined) {
	// 			stk.push(u);
	// 			u = prev[u];
	// 		} 
	// 	}

	// 	auto path = std::vector<int>();
	// 	while (!stk.empty()) {
	// 		path.push_back(stk.top());
	// 		stk.pop();
	// 	}

	// 	return std::pair<std::vector<int>, float>(path, cost);



	// }

} // namespace osm_net