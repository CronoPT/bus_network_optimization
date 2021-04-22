#ifndef __HEAP_TPP__
#define __HEAP_TPP__

namespace net {

	template<typename T, typename H>
	heap<T, H>::heap(): 
	 _vector(), _heap_size(0), _indexes() {
		/* Do Nothing */
	}

	template<typename T, typename H>
	int heap<T, H>::left(int index) const {
		return 2*(index+1) - 1;
	}

	template<typename T, typename H>
	int heap<T, H>::right(int index) const {
		return 2*(index+1);
	}

	template<typename T, typename H>
	int heap<T, H>::parent(int index) const {
		return (index-1)/2;
	}

	template<typename T, typename H>
	int heap<T, H>::size() const {
		return _vector.size();
	}

	template<typename T, typename H>
	void heap<T, H>::min_heapify(int index) {
		// std::cout << "min_heapify" << std::endl;

		int l = left(index);
		int r = right(index);
		int smallest;
		if (l<=(_heap_size-1) && _vector.at(l)<_vector.at(index)) {
			smallest = l; 
		} else { smallest = index; }
		if (r<=(_heap_size-1) && _vector.at(r)<_vector.at(smallest)) {
			smallest = r;
		}

		if (smallest != index) {
			T aux = _vector.at(index);
			_indexes[_vector.at(index)] = smallest;
			_indexes[_vector.at(smallest)] = index;
			_vector.at(index) = _vector.at(smallest);
			_vector.at(smallest) = aux;
			min_heapify(smallest);
		}
	}

	template<typename T, typename H>
	T heap<T, H>::extract_minimum() {
		// std::cout << "extract_minimum" << std::endl;
		T min = _vector.at(0);
		_indexes.erase(min);
		_vector.at(0) = _vector.at(_heap_size-1);
		_indexes[_vector.at(0)] = 0;
		_vector.pop_back();
		_heap_size -= 1;
		min_heapify(0);
		return min;
	}

	template<typename T, typename H>
	void heap<T, H>::insert(T element) {
		// std::cout << "insert" << std::endl;
		_vector.push_back(element);
		_heap_size += 1;
		_indexes[element] = _heap_size-1;
		decrease_key_index(_heap_size-1, element);
	}
	
	template<typename T, typename H>
	void heap<T, H>::decrease_key_index(int index, T element) {
		// std::cout << "decrease_key_index" << std::endl;
		_vector.at(index) = element; 

		// std::cout << "decrease_key_index -> before while" << std::endl;
		while (index > 0 && _vector.at(parent(index))>_vector.at(index)) {
			_indexes[_vector.at(index)] = parent(index);
			// std::cout << "decrease_key_index -> 1" << std::endl;
			_indexes[_vector.at(parent(index))] = index;
			// std::cout << "decrease_key_index -> 2" << std::endl;
			
			T aux = _vector.at(index);
			// std::cout << "decrease_key_index -> 3" << std::endl;
			_vector.at(index) = _vector.at(parent(index));
			// std::cout << "decrease_key_index -> 4" << std::endl;
			_vector.at(parent(index)) = aux;
			// std::cout << "decrease_key_index -> 5" << std::endl;
			index = parent(index);
		}
	}

	template<typename T, typename H>
	void heap<T, H>::decrease_key(T old_element, T element) {
		// std::cout << "decrease_key" << std::endl;
		// std::cout << (old_element == element) << std::endl;
		// std::cout << (std::hash<T>()(old_element) == std::hash<T>()(element)) << std::endl;
		decrease_key_index(_indexes[old_element], element);
	}

	template<typename T, typename H>
	bool heap<T, H>::contains(T element) {
		// std::cout << "contains" << std::endl;
		return _indexes.find(element) != _indexes.end();
	}

	template<typename T, typename H>
	const std::vector<T>& heap<T, H>::get_elements() const {
		// std::cout << "get_elements" << std::endl;
		return _vector;
	}

} // namespace net

#endif