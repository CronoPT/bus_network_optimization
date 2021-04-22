#ifndef __HEAP_TPP__
#define __HEAP_TPP__

namespace net {

	template<typename T>
	heap<T>::heap(): 
	 _vector(), _heap_size(0), _indexes() {
		/* Do Nothing */
	}

	template<typename T>
	int heap<T>::left(int index) const {
		return 2*(index+1) - 1;
	}

	template<typename T>
	int heap<T>::right(int index) const {
		return 2*(index+1);
	}

	template<typename T>
	int heap<T>::parent(int index) const {
		return (index-1)/2;
	}

	template<typename T>
	void heap<T>::min_heapify(int index) {
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

	template<typename T>
	T heap<T>::extract_minimum() {
		T min = _vector.at(0);
		_indexes.erase(min);
		_vector.at(0) = _vector.at(_heap_size-1);
		_heap_size -= 1;
		min_heapify(0);
		return min;
	}

	template<typename T>
	void heap<T>::insert(T element) {
		_heap_size += 1;
		_vector.push_back(element);
		_indexes[element] = _heap_size-1;
		decrease_key_index(_heap_size-1, element);
	}
	
	template<typename T>
	void heap<T>::decrease_key_index(int index, T element) {
		_vector.at(index) = element; 

		while (index > 0 && _vector.at(parent(index))>_vector.at(index)) {
			_indexes[_vector.at(index)] = parent(index);
			_indexes[_vector.at(parent(index))] = index;
			
			T aux = _vector.at(index);
			_vector.at(index) = _vector.at(parent(index));
			_vector.at(parent(index)) = aux;
			index = parent(index);
		}
	}

	template<typename T>
	void heap<T>::decrease_key(T old_element, T element) {
		decrease_key_index(_indexes[old_element], element);
	}

} // namespace net

#endif