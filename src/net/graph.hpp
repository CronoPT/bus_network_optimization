#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <unordered_map>
#include <queue>
#include <limits>
#include <vector>
#include <stack>
#include "node.hpp"
#include "edge.hpp"
#include "priority_queue.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

namespace net {

	template<typename V, typename E>
	class graph {
		private:
		std::unordered_map<int, node<V, E>> _nodes;
		std::unordered_map<int, edge<E>> _edges;

		public:
		graph();
		void add_node(int id);
		void add_node(int id, V attributes);
		void add_edge(int origin_id, int destin_id);
		void add_edge(int origin_id, int destin_id, E attributes);

		int get_number_of_nodes();
		int get_number_of_edges();

		std::unordered_map<int, node<V, E>>& get_nodes();
		std::unordered_map<int, edge<E>>& get_edges();

		std::pair<std::vector<int>, float> dijkstra(
			int origin_id, 
			int destin_id,
			float (*weight)(edge<E>&)
		);

	};

} // namespace net

#include "graph.tpp"

#endif