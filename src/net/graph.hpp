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

		std::unordered_map<int, int> _node_to_index;
		int _total_nodes;

		std::vector<float> _dist;
		std::vector<int>   _prev;
		std::vector<int>   _extra;

		bool _nodes_added;

		public:
		graph();
		void add_node(int id);
		void add_node(int id, V attributes);
		void add_edge(int origin_id, int destin_id);
		void add_edge(int origin_id, int destin_id, E attributes);

		void remove_edge(
			int origin_id, 
			int destin_id, 
			int id
		);
		void remove_node(int node_id);

		int get_number_of_nodes();
		int get_number_of_edges();
		bool has_node(int node_id);
		bool has_edge_between(int n1, int n2);

		inline void mark_dist(int node_id, float distance);
		inline void mark_prev(int node_id, int prev);
		inline void mark_extra(int node_id, int ex);
		inline float get_dist(int node_id);
		inline int   get_prev(int node_id);
		inline int   get_extra(int node);

		inline void add_nodes_check();

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