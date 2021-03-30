#ifndef __EDGE_TPP__
#define __EDGE_TPP__

namespace net {

	template<typename E>
	edge<E>::edge(int origin, int destin, int key, int id, E& attributes):
	 _origin(origin), _destin(destin), _key(key), _id(id),
	 _attributes(attributes) {

	}

	template<typename E>
	edge<E>::edge():
	 _origin(-1), _destin(-1), _key(-1), _id(-1),
	 _attributes() {

	}


	template<typename E>
	int edge<E>::get_id() const {
		return _id;
	}

	template<typename E>
	int edge<E>::get_origin() const {
		return _origin;
	}

	template<typename E>
	int edge<E>::get_destin() const {
		return _destin;
	}

	template<typename E>
	int edge<E>::get_key() const {
		return _key;
	}

	template<typename E>
	E& edge<E>::get_attributes() {
		return _attributes;
	}


} // namespace net

#endif