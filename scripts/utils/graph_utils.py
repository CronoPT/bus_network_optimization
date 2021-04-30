import networkx as nx
import numpy as np

def remove_edge_on_shortest_path(G, u, v):
	best_len = np.inf
	best_tup = (None, None, None, None)
	# print(f'Attempting to remove edge ({u},{v}) -> {G.get_edge_data(u, v)}')
	for key, attr in G.get_edge_data(u, v).items():
		if attr['length'] < best_len:
			best_len = attr['length']
			best_tup = (u, v, key, attr)

	G.remove_edge(u, v, best_tup[2])
	return best_tup

def remove_node_and_record_edges(G, node):
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
	# print(path)
	# print(G.edges)
	origin = None
	total_cost = 0
	for destin in path:	
		if origin != None:
			min = np.inf
			# print(f'Origin: {origin} | Destin: {destin} | TotalCost: {total_cost}')
			for _, attr in G.get_edge_data(origin, destin).items():
				if attr['length'] < min:
					min = attr['length']
			total_cost += min

		origin = destin
	
	return total_cost

def k_shortest_paths(G, source, target, K, weight='length'):

	shortest_path = nx.algorithms.shortest_paths.weighted.dijkstra_path(
		G, 
		source=source, 
		target=target,
		weight=weight
	)
	
	candidates = []
	As = [shortest_path]
	for k in range(1, K, 1):
		for i in range(0, len(As[k-1])-1):
			deleted_edges = []
			deleted_nodes = []

			spur_node = As[k-1][i]
			# print('=====================')
			# print(f'As[k-1] = {As[k-1]}')
			# print(f'Spur Node: {spur_node}')
			root_path = As[k-1][0:i+1]
			root_path_cost = compute_path_cost(G, root_path)
			# print(f'Root path: {root_path}')
			
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
				# print(f'Spur path: {spur_path}')
				total_path = root_path + spur_path[1:]
				if total_path not in candidates:
					candidates.append((total_path, root_path_cost+spur_path_cost))
					# print(f'Total path: {total_path}')
			except nx.NetworkXNoPath:
				print('Path not found, no biggie')

			# print(f'Deleted Edges: {deleted_edges}')
			# print('=====================')	

			for node in deleted_nodes:
				G.add_node(node[0], **node[1])
			for edge in deleted_edges:
				if 'key' in edge[3]:
					G.add_edge(edge[0], edge[1], **edge[3])
				else:
					G.add_edge(edge[0], edge[1], key=edge[2], **edge[3])

		if len(candidates) > 0:
			# print(f'Candidates -> {candidates}')
			candidates.sort(key = lambda x: x[1])
			As.append(candidates[0][0])
			candidates = candidates[1:]
		else:
			break
	return As