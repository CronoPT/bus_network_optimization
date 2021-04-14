
import json
import networkx as nx


def read_json_object(path):
	file = open(path, 'r')
	json_object = json.load(file)
	file.close()
	return json_object


def write_json_object(path, json_object):
	with open(path, 'w') as json_file:
		json.dump(json_object, json_file, indent=4)


def write_geojson_lines(path, lines_list):
	geojson_object = {	
		'type': 'MultiLineString',
		'coordinates': lines_list
	}
	write_json_object(path, geojson_object)


def write_geojson_points(path, points_list):
	geojson_object = {
		'type': 'MultiPoint',
		'coordinates': points_list
	}
	write_json_object(path, geojson_object)


def read_network_json(path):
	json_network = read_json_object(path)
	network = nx.readwrite.json_graph.adjacency_graph(json_network)
	return network


def write_networkx_json(path, network):
	json_network = nx.readwrite.json_graph.adjacency_data(network)
	write_json_object(path, json_network)