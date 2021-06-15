#ifndef __GRAPH_TPP__
#define __GRAPH_TPP__

namespace net {

	template<typename V, typename E>
	graph<V, E>::graph(): 
	 _nodes(), _edges(), _node_to_index(),
	 _total_nodes(0), _dist(), _prev(),
	 _extra(), _nodes_added(true) {
		/* Do Nothing */
	}

	template<typename V, typename E>
	void graph<V, E>::add_node(int id) {
		auto vertex = node<V, E>(id);
		_nodes[id]  = vertex;
		_node_to_index[id] = _total_nodes++;
		_nodes_added = true;
	}

	template<typename V, typename E>
	void graph<V, E>::add_node(int id, V attributes) {
		auto vertex = node<V, E>(id, attributes);
		_nodes[id]  = vertex;
		_node_to_index[id] = _total_nodes++;
		_nodes_added = true;
	}

	template<typename V, typename E>
	void graph<V, E>::add_edge(int origin_id, int destin_id) {
		if (_nodes.find(origin_id) == _nodes.end()) {
			add_node(origin_id);
		}

		if (_nodes.find(destin_id) == _nodes.end()) {
			add_node(destin_id);
		}

		int key = 0;
		int id  = destin_id*10 + key;

		while (_edges.find(id) != _edges.end()) {
			key += 1;
			id   = destin_id*10 + key;
		}

		auto link = edge<E>(origin_id, destin_id, key, id);

		_edges[id] = link;
		_nodes[origin_id].add_adjacency(id, link);
	}

	template<typename V, typename E>
	void graph<V, E>::add_edge(int origin_id, int destin_id, E attributes) {
		if (_nodes.find(origin_id) == _nodes.end()) {
			add_node(origin_id);
		}

		if (_nodes.find(destin_id) == _nodes.end()) {
			add_node(destin_id);
		}

		int key = 0;
		int id  = destin_id*10 + key;

		while (_edges.find(id) != _edges.end()) {
			key += 1;
			id   = destin_id*10 + key;
		}

		auto link = edge<E>(origin_id, destin_id, key, id, attributes);

		_edges[id] = link;
		_nodes[origin_id].add_adjacency(id, link);
	}

	template<typename V, typename E>
	void graph<V, E>::remove_edge(
		int origin_id, 
		int destin_id, 
		int id
	) {
		_nodes[origin_id].get_adjacencies().erase(id);
		_edges.erase(id);
	}

	template<typename V, typename E>
	void graph<V, E>::remove_node(int node_id) {
		/**
		 * WARNING: the edge clean up must be
		 * performed by the user for the time
		 * being
		*/
		_nodes.erase(node_id);
	}

	template<typename V, typename E>
	int graph<V, E>::get_number_of_nodes() {
		return _nodes.size();
	}

	template<typename V, typename E>
	int graph<V, E>::get_number_of_edges() {
		return _edges.size();
	}

	template<typename V, typename E>
	bool graph<V, E>::has_node(int node_id) {
		return _nodes.find(node_id) != _nodes.end();
	}

	template<typename V, typename E>
	bool graph<V, E>::has_edge_between(int n1, int n2) {
		auto& adjs = _nodes[n1].get_adjacencies();
		for (auto& e: adjs) {
			auto& link = e.second;
			if (link.get_destin() == n2) { return true; }
		}
		return false;
	}

	template<typename V, typename E>
	inline void graph<V, E>::mark_dist(int node_id, float distance) {
		add_nodes_check();
		int index = _node_to_index[node_id];
		_dist[index] = distance;
	}

	template<typename V, typename E>
	inline void graph<V, E>::mark_prev(int node_id, int prev) {
		add_nodes_check();
		int index = _node_to_index[node_id];
		_prev[index] = prev;
	}

	template<typename V, typename E>
	inline void graph<V, E>::mark_extra(int node_id, int ex) {
		add_nodes_check();
		int index = _node_to_index[node_id];
		_extra[index] = ex;
	}

	template<typename V, typename E>
	inline float graph<V, E>::get_dist(int node_id) {
		add_nodes_check();
		int index = _node_to_index[node_id];
		return _dist[index];
	}

	template<typename V, typename E>
	inline int graph<V, E>::get_prev(int node_id) {
		add_nodes_check();
		int index = _node_to_index[node_id];
		return _prev[index];
	}

	template<typename V, typename E>
	inline int graph<V, E>::get_extra(int node_id) {
		add_nodes_check();
		int index = _node_to_index[node_id];
		return _extra[index];
	}

	template<typename V, typename E>
	inline int graph<V, E>::get_node_index(int node_id) {
		add_nodes_check();
		return _node_to_index[node_id];
	}

	template<typename V, typename E>
	inline void graph<V, E>::add_nodes_check() {
		if (_nodes_added) {
			_dist  = std::vector<float>(_total_nodes, 0);
			_prev  = std::vector<int>(_total_nodes, 0);
			_extra = std::vector<int>(_total_nodes, 0);
			_nodes_added = false;
		}
	}

	template<typename V, typename E>
	std::unordered_map<int, node<V, E>>& graph<V, E>::get_nodes() {
		return _nodes;
	}

	template<typename V, typename E>
	std::unordered_map<int, edge<E>>& graph<V, E>::get_edges() {
		return _edges;
	}

	template<typename V, typename E>
	std::pair<std::vector<int>, float> graph<V, E>::dijkstra(
		int origin_id, 
		int destin_id,
		float (*weight)(edge<E>&)
	) {
		add_nodes_check();

		auto queue = net::priority_queue<int>();
		auto explo = std::set<int>();
		
		const int undefined = -1;

		auto i_origin = _node_to_index[origin_id];
		_prev[i_origin] = undefined;
		_dist[i_origin] = 0.0;
		queue.push(origin_id, 0.0);

		while (!queue.empty()) {

			auto u = queue.pop();
			explo.insert(u);

			if (u == destin_id) { break; }

			auto  u_i = _node_to_index[u];
			auto& u_node = _nodes[u];
			for (auto& edge_info: u_node.get_adjacencies()) {
				auto   key  = edge_info.first;
				auto&  edge = edge_info.second;
				auto   v    = edge.get_destin(); 
				auto   v_i  = _node_to_index[v];

				if (queue.contains(v)) {
					float alt = _dist[u_i] + weight(edge);
					if (alt < _dist[v_i]) {
						_dist[v_i] = alt;
						_prev[v_i] = u;
						queue.update(v, alt);
					}
				} else if (explo.find(v) == explo.end()) {
					/* v hasn't been visited before */
					_dist[v_i] = _dist[u_i] + weight(edge);
					_prev[v_i] = u;
					queue.push(v, _dist[v_i]);
				}
			}

		}

		auto i_destin = _node_to_index[destin_id];
		float cost = _dist[i_destin];
		auto stk   = std::stack<int>();
		int  u   = destin_id;
		int  u_i = _node_to_index[u];
		if (_prev[u_i]!=undefined || u==origin_id) {
			while (u != undefined) {
				stk.push(u);
				u   = _prev[u_i];
				u_i = _node_to_index[u]; 
			} 
		}

		auto path = std::vector<int>();
		while (!stk.empty()) {
			path.push_back(stk.top());
			stk.pop();
		}

		return std::pair<std::vector<int>, float>(path, cost);
	}

} // namespace net

#endif