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
	item<T>::item(T key, float cost):
	 _cost(cost), _key(key) {

	}

	template<typename T>
	priority_queue<T>::priority_queue():
	 _set() {

	}

	template<typename T>
	void priority_queue<T>::push(T key, float cost) {
		auto holder = item<T>(key, cost);
		_set.insert(holder);
	}

	template<typename T>
	T priority_queue<T>::pop() {
		auto holder = *_set.begin();
		_set.erase(holder);
		return holder._key;
	}

	template<typename T>
	void priority_queue<T>::update(T key, float new_cost) {
		auto it = std::find_if(_set.begin(), _set.end(), [key](item<T> holder){
			return holder._key == key;
		});
		if (it != _set.end()) {
			_set.erase(it);
			push(key, new_cost);
		}
	}

	template<typename T>
	int priority_queue<T>::size() {
		return _set.size();
	}

	template<typename T>
	bool priority_queue<T>::empty() {
		return _set.empty();
	}

	template<typename T>
	bool priority_queue<T>::contains(T key) {
		auto it = std::find_if(_set.begin(), _set.end(), [key](item<T> holder){
			return holder._key == key;
		});
		return it != _set.end();
	}

	template<typename T>
	void priority_queue<T>::print() {
		for (auto i: _set) {
			std::cout << "id: " << i._key 
			          << "cost: " << i._cost << "\n";
		}
		std::cout << std::endl;
	}


} // namespace net

#endif