#ifndef __NODE_TPP__
#define __NODE_TPP__

namespace net {

	template<typename V, typename E>
	node<V, E>::node(int id, V& attributes): 
	 _id(id), _attributes(attributes), _adjacencies() {

	}

	template<typename V, typename E>
	node<V, E>::node(int id): 
	 _id(id), _attributes(), _adjacencies() {

	}

	template<typename V, typename E>
	node<V, E>::node(): 
	 _id(-1), _attributes(), _adjacencies() {

	}

	template<typename V, typename E>
	int node<V, E>::get_id() const {
		return _id;
	}

	template<typename V, typename E>
	std::unordered_map<int, edge<E>>& node<V, E>::get_adjacencies() {
		return _adjacencies;
	}

	template<typename V, typename E>
	V& node<V, E>::get_attributes() {
		return _attributes;
	} 

	template<typename V, typename E>
	bool node<V, E>::is_of_type(std::string type) {
		return (type=="any" || _attributes.get_type()==type);
	}

	template<typename V, typename E>
	std::vector<edge<E>> node<V, E>::get_out_edges() {
		auto edge_vec = std::vector<edge<E>>();
		for (auto& edge_info: _adjacencies) {
			edge_vec.push_back(edge_info.second);
		}
		return edge_vec;
	}

	template<typename V, typename E>
	void node<V, E>::add_adjacency(int key, edge<E>& edge) {
		_adjacencies[key] = edge;
	}

	template<typename V, typename E>
	void node<V, E>::remove_adjacency(int id) {
		_adjacencies.erase(id);
	}

} // namespace net

#endif