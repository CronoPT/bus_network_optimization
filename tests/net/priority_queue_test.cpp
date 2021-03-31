#include <gtest/gtest.h>
#include <priority_queue.hpp>

class priority_queue_test: public testing::Test {  

	protected:
		net::priority_queue<int> _queue;

};

TEST_F(priority_queue_test, priority_queue_simple_test) {

	_queue = net::priority_queue<int>();

	ASSERT_EQ(_queue.size(), 0);
	ASSERT_TRUE(_queue.empty());

	_queue.push(10, 2.0);
	_queue.push(40, 4.0);
	_queue.push(30, 3.0);

	ASSERT_EQ(_queue.size(), 3);
	ASSERT_FALSE(_queue.empty());
	ASSERT_EQ(_queue.pop(), 10);
	ASSERT_EQ(_queue.pop(), 30);
	ASSERT_EQ(_queue.pop(), 40);
	ASSERT_EQ(_queue.size(), 0);
	ASSERT_TRUE(_queue.empty());

}

TEST_F(priority_queue_test, priority_queue_update_test) {

	_queue = net::priority_queue<int>();

	ASSERT_EQ(_queue.size(), 0);
	ASSERT_TRUE(_queue.empty());

	_queue.push(10, 2.0);
	_queue.push(40, 4.0);
	_queue.push(30, 3.0);

	_queue.update(10, 50);

	ASSERT_TRUE(_queue.contains(10));
	ASSERT_TRUE(_queue.contains(40));
	ASSERT_TRUE(_queue.contains(30));
	ASSERT_FALSE(_queue.contains(60));

	ASSERT_EQ(_queue.size(), 3);
	ASSERT_FALSE(_queue.empty());
	ASSERT_EQ(_queue.pop(), 30);
	ASSERT_EQ(_queue.pop(), 40);
	ASSERT_EQ(_queue.pop(), 10);
	ASSERT_EQ(_queue.size(), 0);
	ASSERT_TRUE(_queue.empty());

}
