#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <unordered_map>
#include <vector>
#include <string>
#include "edge.hpp"

namespace net {

	template<typename V, typename E>
	class node {
		private:
		int _id;
		std::unordered_map<int, edge<E>> _adjacencies;
		V _attributes;

		public:
		node(int id, V& attributes);
		node(int id);
		node();

		int get_id() const;
		std::unordered_map<int, edge<E>>& get_adjacencies();
		V& get_attributes();

		bool is_of_type(std::string type); 

		std::vector<edge<E>> get_out_edges();
		void add_adjacency(int key, edge<E>& edge);
		
	};

} // namespace net

#include "node.tpp"

#endif