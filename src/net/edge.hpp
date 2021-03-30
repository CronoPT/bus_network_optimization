#ifndef __EDGES_HPP__
#define __EDGES_HPP__

namespace net {

	template<typename E>
	class edge {
		private:
		E _attributes;
		int _id;
		int _origin;
		int _destin;
		int _key;

		public:
		edge(int origin, int destin, int key, int id, E& attributes);
		edge();
		int get_id() const;
		int get_origin() const;
		int get_destin() const;
		int get_key() const;
		E&  get_attributes();
	};

} // namespace net

#include "edge.tpp"

#endif