
import utils.json_utils
import networkx as nx
import time

if __name__ == '__main__':
	network = utils.json_utils.read_network_json("../data/json/final_network.json")
	routes  = utils.json_utils.read_json_object("../data/json/clustered_routes_stop_sequence.json")
	path_count = 0

	start = time.time()

	for route in routes:
		previous = None
		for stop in route['stops']:
			if previous != None:
				path_count += 1
				path = nx.algorithms.shortest_paths.weighted.dijkstra_path(
					network, 
					source=previous, 
					target=stop,
					weight='length'
				)
			previous = stop

	end = time.time()

	print(f"Computed {path_count} paths in {end-start} seconds")
	print(f"Nodes in network: {network.number_of_nodes()}")
	print(f"Edges in network: {network.number_of_edges()}")
