import networkx as nx
import numpy as np

def remove_edge_on_shortest_path(G, u, v):
	'''
	| Remove an edge between two nodes.
	| If multi-edges exist, remove the 
	| lowest cost one. Then return the 
	| edge information so that it can
	| be put back in the graph later.
	'''
	best_len = np.inf
	best_tup = (None, None, None, None)
	for key, attr in G.get_edge_data(u, v).items():
		if attr['length'] < best_len:
			best_len = attr['length']
			best_tup = (u, v, key, attr)

	G.remove_edge(u, v, best_tup[2])
	return best_tup

def remove_node_and_record_edges(G, node):
	'''
	| Remove a node and its edges from a graph
	| but record the deleted edges so that the
	| node can be put back in graph later.
	'''
	deleted_edges = []

	for edge in G.out_edges(G, node):
		info = G.get_edge_data(*edge)
		for key, attr in info.items():
			deleted_edges.append((
				edge[0], edge[1], key, attr
			))
	for edge in G.in_edges(G, node):
		info = G.get_edge_data(*edge)
		for key, attr in info.items():
			deleted_edges.append((
				edge[0], edge[1], key, attr
			))
	
	G.remove_node(node)
	return deleted_edges

def compute_path_cost(G, path):
	'''
	| Compute path cost assuming that it is a minimal path,
	| so, every, when multi-edges exist, assume that the one
	| on the path is the lower cost one.
	'''
	origin = None
	total_cost = 0
	for destin in path:	
		if origin != None:
			min = np.inf
			for _, attr in G.get_edge_data(origin, destin).items():
				if attr['length'] < min:
					min = attr['length']
			total_cost += min

		origin = destin
	
	return total_cost

def k_shortest_paths(G, source, target, K, weight='length'):
	'''
	| Implementation of Yen's algorithm to compute k-shortest
	| paths between two nodes in a directed multi graph.
	| Some of the additional clutter and functions come from
	| the fact that this was programmed to work with
	| NetworkX's MultiDiGraph.
	|
	| The code was based off of this article: 
	| https://en.wikipedia.org/wiki/Yen%27s_algorithm
	'''
	try:
		shortest_path = nx.algorithms.shortest_paths.weighted.dijkstra_path(
			G, 
			source=source, 
			target=target,
			weight=weight
		)
	except nx.NetworkXNoPath:
		return []
	
	candidates = []
	As = [shortest_path]
	for k in range(1, K, 1):
		for i in range(0, len(As[k-1])-1):
			# Auxiliary structures so we can restore
			# removed edges and nodes later.
			deleted_edges = []
			deleted_nodes = []

			spur_node = As[k-1][i]
			root_path = As[k-1][0:i+1]
			root_path_cost = compute_path_cost(G, root_path)
			
			for path in As:
				if root_path == path[0:i+1]:
					u = path[i]
					v = path[i+1]
					if G.get_edge_data(u, v) != None:
						info = remove_edge_on_shortest_path(G, u, v)
						deleted_edges.append(info)

			for node in root_path:
				if node != spur_node and node in G.nodes:
					deleted_nodes.append((node, G.nodes[node]))
					info = remove_node_and_record_edges(G, node)
					deleted_edges.extend(info)

			try:
				spur_path = nx.algorithms.shortest_paths.weighted.dijkstra_path(
					G, 
					source=spur_node, 
					target=target,
					weight=weight
				)
				spur_path_cost = compute_path_cost(G, spur_path)
				total_path = root_path + spur_path[1:]
				if total_path not in candidates:
					candidates.append((total_path, root_path_cost+spur_path_cost))
			except nx.NetworkXNoPath:
				# Nothing to do hear, if a path does not exist with
				# all the removed edges and nodes, it is not deal
				# breaking.
				pass	

			for node in deleted_nodes:
				G.add_node(node[0], **node[1])
			for edge in deleted_edges:
				if 'key' in edge[3]:
					G.add_edge(edge[0], edge[1], **edge[3])
				else:
					G.add_edge(edge[0], edge[1], key=edge[2], **edge[3])

		if len(candidates) > 0:
			candidates.sort(key = lambda x: x[1])
			As.append(candidates[0][0])
			candidates = candidates[1:]
		else:
			break
	return As