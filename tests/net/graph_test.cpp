#include <gtest/gtest.h>
#include <graph.hpp>
#include <edge.hpp>

class num_holder {
	int _number;

	public:
		num_holder(int num): _number(num) {

		}

		num_holder(): _number(0) {

		}

		int get_num() { return _number; }
};


class graph_test: public testing::Test {  

	protected:
		net::graph<num_holder, num_holder> _G;

};

TEST_F(graph_test, add_adjacencies_test) {
	_G.add_node(0);
	_G.add_node(1);
	_G.add_node(2);

	_G.add_edge(0, 1, num_holder(4));
	_G.add_edge(0, 2, num_holder(8));

	auto adjacen = _G.get_nodes()[0].get_adjacencies();

	ASSERT_EQ(adjacen[10].get_attributes().get_num(), 4);
	ASSERT_EQ(adjacen[20].get_attributes().get_num(), 8);
}

TEST_F(graph_test, remove_adjacencies_simple_test) {
	_G.add_node(0);
	_G.add_node(1);
	_G.add_node(2);

	_G.add_edge(0, 1, num_holder(4));
	_G.add_edge(0, 2, num_holder(8));

	auto& adjacen = _G.get_nodes()[0].get_adjacencies();

	ASSERT_EQ(adjacen[10].get_attributes().get_num(), 4);
	ASSERT_EQ(adjacen[20].get_attributes().get_num(), 8);

	_G.remove_edge(0, 2, 20);

	ASSERT_EQ(adjacen[10].get_attributes().get_num(), 4);
	ASSERT_EQ(_G.get_number_of_edges(), 1);
	ASSERT_EQ(adjacen.find(20), adjacen.end());
}

TEST_F(graph_test, dijkstra_test) {

	_G.add_node(0);
	_G.add_node(1);
	_G.add_node(2);
	_G.add_node(3);
	_G.add_node(4);
	_G.add_node(5);
	_G.add_node(6);
	_G.add_node(7);
	_G.add_node(8);

	_G.add_edge(0, 1, num_holder(4));
	_G.add_edge(1, 2, num_holder(8));
	_G.add_edge(2, 3, num_holder(7));
	_G.add_edge(3, 4, num_holder(9));
	_G.add_edge(0, 7, num_holder(8));
	_G.add_edge(7, 6, num_holder(1));
	_G.add_edge(6, 5, num_holder(2));
	_G.add_edge(5, 4, num_holder(10));
	_G.add_edge(1, 7, num_holder(11));
	_G.add_edge(7, 8, num_holder(7));
	_G.add_edge(8, 6, num_holder(6));
	_G.add_edge(2, 8, num_holder(2));
	_G.add_edge(2, 5, num_holder(4));
	_G.add_edge(3, 5, num_holder(14));

	auto res = _G.dijkstra(0, 4, [](net::edge<num_holder>& e) -> float {
		return (float) e.get_attributes().get_num();
	});

	ASSERT_EQ(res.first, std::vector<int>({0, 7, 6, 5, 4}));
	ASSERT_EQ(res.second, 21.0);
}

TEST_F(graph_test, dijkstra_multi_test) {

	_G.add_node(0);
	_G.add_node(1);
	_G.add_node(2);
	_G.add_node(3);
	_G.add_node(4);
	_G.add_node(5);
	_G.add_node(6);
	_G.add_node(7);
	_G.add_node(8);

	_G.add_edge(0, 1, num_holder(4));
	_G.add_edge(1, 2, num_holder(8));
	_G.add_edge(1, 2, num_holder(1));
	_G.add_edge(2, 3, num_holder(7));
	_G.add_edge(2, 3, num_holder(1));
	_G.add_edge(3, 4, num_holder(9));
	_G.add_edge(0, 7, num_holder(8));
	_G.add_edge(7, 6, num_holder(1));
	_G.add_edge(6, 5, num_holder(2));
	_G.add_edge(5, 4, num_holder(10));
	_G.add_edge(1, 7, num_holder(11));
	_G.add_edge(7, 8, num_holder(7));
	_G.add_edge(8, 6, num_holder(6));
	_G.add_edge(2, 8, num_holder(2));
	_G.add_edge(2, 5, num_holder(4));
	_G.add_edge(3, 5, num_holder(14));

	auto res = _G.dijkstra(0, 4, [](net::edge<num_holder>& e) -> float {
		return (float) e.get_attributes().get_num();
	});

	ASSERT_EQ(res.first, std::vector<int>({0, 1, 2, 3, 4}));
	ASSERT_EQ(res.second, 15.0);
}

TEST_F(graph_test, dijkstra_multi_trick_test) {

	_G.add_node(0);
	_G.add_node(1);
	_G.add_node(2);

	_G.add_edge(0, 1, num_holder(1));
	_G.add_edge(0, 2, num_holder(5));
	_G.add_edge(1, 2, num_holder(1));


	auto res = _G.dijkstra(0, 2, [](net::edge<num_holder>& e) -> float {
		return (float) e.get_attributes().get_num();
	});

	ASSERT_EQ(res.first, std::vector<int>({0, 1, 2}));
	ASSERT_EQ(res.second, 2.0);
}

TEST_F(graph_test, remove_adjacencies_test) {
	_G.add_node(0);
	_G.add_node(1);
	_G.add_node(2);
	_G.add_node(3);

	_G.add_edge(0, 1, num_holder(1));
	_G.add_edge(0, 2, num_holder(6));
	_G.add_edge(1, 2, num_holder(1));
	_G.add_edge(0, 3, num_holder(2));
	_G.add_edge(3, 2, num_holder(2));

	auto& adjacen = _G.get_nodes()[0].get_adjacencies();

	auto res = _G.dijkstra(0, 2, [](net::edge<num_holder>& e) -> float {
		return (float) e.get_attributes().get_num();
	});
	ASSERT_EQ(res.first, std::vector<int>({0, 1, 2}));
	ASSERT_EQ(res.second, 2.0);

	_G.remove_edge(0, 1, 10);
	res = _G.dijkstra(0, 2, [](net::edge<num_holder>& e) -> float {
		return (float) e.get_attributes().get_num();
	});
	ASSERT_EQ(res.first, std::vector<int>({0, 3, 2}));
	ASSERT_EQ(res.second, 4.0);

	_G.remove_edge(0, 3, 30);
	res = _G.dijkstra(0, 2, [](net::edge<num_holder>& e) -> float {
		return (float) e.get_attributes().get_num();
	});
	ASSERT_EQ(res.first, std::vector<int>({0, 2}));
	ASSERT_EQ(res.second, 6.0);

	_G.add_edge(0, 3, num_holder(2));
	_G.add_edge(0, 1, num_holder(1));
	res = _G.dijkstra(0, 2, [](net::edge<num_holder>& e) -> float {
		return (float) e.get_attributes().get_num();
	});
	ASSERT_EQ(res.first, std::vector<int>({0, 1, 2}));
	ASSERT_EQ(res.second, 2.0);
}