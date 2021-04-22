#ifndef __PRIORITY_QUEUE_TPP__
#define __PRIORITY_QUEUE_TPP__

namespace net {

	template<typename T>
	bool operator<(const item<T>& i1, const item<T>& i2) {
		if (i1._cost == i2._cost) {
			return i1._key < i2._key;
		}
		else {
			return i1._cost < i2._cost;
		}
	}

	template<typename T>
	bool operator>(const item<T>& i1, const item<T>& i2) {
		if (i1._cost == i2._cost) {
			return i1._key > i2._key;
		}
		else {
			return i1._cost > i2._cost;
		}
	}

	template<typename T>
	bool operator==(const item<T>& i1, const item<T>& i2) {
		return i1._key == i2._key;
	}

	template<typename T>
	item<T>::item(T key, float cost):
	 _cost(cost), _key(key) {

	}

	template<typename T>
	priority_queue<T>::priority_queue():
	 _heap() {

	}

	template<typename T>
	void priority_queue<T>::push(T key, float cost) {
		auto holder = item<T>(key, cost);
		_heap.insert(holder);
	}

	template<typename T>
	T priority_queue<T>::pop() {
		auto holder = _heap.extract_minimum();
		return holder._key;
	}

	template<typename T>
	void priority_queue<T>::update(T key, float new_cost) {
		auto holder = item<T>(key, new_cost);
		_heap.decrease_key(holder, holder);
	}

	template<typename T>
	int priority_queue<T>::size() {
		return _heap.size();
	}

	template<typename T>
	bool priority_queue<T>::empty() {
		return size() == 0;
	}

	template<typename T>
	bool priority_queue<T>::contains(T key) {
		auto holder = item<T>(key, 0);
		return _heap.contains(holder);
	}

	template<typename T>
	void priority_queue<T>::print() {
		for (const auto& i: _heap.get_elements()) {
			std::cout << "id: " << i._key 
			          << "cost: " << i._cost << "\n";
		}
		std::cout << std::endl;
	}


} // namespace net

#endif