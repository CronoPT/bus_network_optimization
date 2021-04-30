import unittest
import networkx as nx
import utils.graph_utils

class test_yens_algorithm(unittest.TestCase):

	def setUp(self):
		self.G = nx.MultiDiGraph()
		self.G.add_edge('C', 'D', length=3)
		self.G.add_edge('C', 'E', length=2)
		self.G.add_edge('E', 'D', length=1)
		self.G.add_edge('E', 'F', length=2)
		self.G.add_edge('E', 'G', length=3)
		self.G.add_edge('D', 'F', length=4)
		self.G.add_edge('F', 'G', length=2)
		self.G.add_edge('F', 'H', length=1)
		self.G.add_edge('G', 'H', length=2)

	def test_wikipedia_case(self):
		paths = utils.graph_utils.k_shortest_paths(
			self.G, 'C', 'H', 3, weight='length'
		)

		self.assertIn(['C', 'E', 'F', 'H'], paths)
		self.assertIn(['C', 'E', 'G', 'H'], paths)
		self.assertIn(['C', 'D', 'F', 'H'], paths)