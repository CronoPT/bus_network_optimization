
import utils.json_utils
import utils.graph_utils
import configs
import numpy as np
import math
import random 

MIN_ROUTE_LEN = np.inf 
MAX_ROUTE_LEN = -np.inf
DIVISIONS = 30

max_lat = -np.inf
min_lat = np.inf
max_lon = -np.inf
min_lon = np.inf

lat_step = 0
lon_step = 0

main_squares = [
	[17, 17], # Sete Rios
	[17, 15], # Campolide
	[21, 11], # Cais do Sodre
	[19, 22], # Campo Grande
	[11, 12], # Alto da Ajuda
	[11, 10]  # Belem
]

def coord_to_square(lon, lat):
	'''
	| This function maps a coordinate pair to 
	| a x&y position in the greed.
	| x indexes longitude and y indexes latitude.
	'''

	global lon_step
	global lat_step
	global DIVISIONS

	x = math.floor( (lon-min_lon) / lon_step )
	y = math.floor( (lat-min_lat) / lat_step )
	x = x if x < DIVISIONS else x-1
	y = y if y < DIVISIONS else y-1

	y = DIVISIONS - 1 - y

	return x, y

def assert_limits(routes):
	global MIN_ROUTE_LEN
	global MAX_ROUTE_LEN

	for route_item in routes:
		route_len = len(route_item['stops'])

		if route_len > MAX_ROUTE_LEN:
			MAX_ROUTE_LEN = route_len
		
		if route_len < MIN_ROUTE_LEN:
			MIN_ROUTE_LEN = route_len

def get_adjacent_squares(i, j):
	candidate_coords = [[i, j]]
	
	if i > 0:
		candidate_coords.append([i-1, j])

	if j > 0:
		candidate_coords.append([i, j-1])

	if i < DIVISIONS-1:
		candidate_coords.append([i+1, j])
	
	if j < DIVISIONS-1:
		candidate_coords.append([i, j+1])

	if i>0 and j>0:
		candidate_coords.append([i-1, j-1])

	if i<DIVISIONS-1 and j<DIVISIONS-1:
		candidate_coords.append([i+1, j+1])

	if i>0 and j<DIVISIONS-1:
		candidate_coords.append([i-1, j+1])

	if i<DIVISIONS-1 and j>0:
		candidate_coords.append([i+1, j-1])

	return candidate_coords


def assert_coordinate_limits(net):
	global max_lat
	global min_lat
	global max_lon
	global min_lon
	global lat_step
	global lon_step

	for node in net['nodes']:
		lon = node['x']
		lat = node['y']

		max_lat = lat if lat > max_lat else max_lat
		min_lat = lat if lat < min_lat else min_lat
		max_lon = lon if lon > max_lon else max_lon
		min_lon = lon if lon < min_lon else min_lon

	lat_step = (max_lat-min_lat)/DIVISIONS
	lon_step = (max_lon-min_lon)/DIVISIONS


def choose_route_length():
	return random.randint(MIN_ROUTE_LEN, MAX_ROUTE_LEN)


def add_k_shortest():
	global route_pool
	global main_squares
	global grid
	global road_network
	global existing_stops

	for or_i, or_j in main_squares:
		for de_i, de_j in main_squares:
			if i != j:
				origin = None
				destin = None
				if len(grid[or_i][or_j]) > 1:
					origin = grid[or_i][or_j][random.randint(0, len(grid[or_i][or_j])-1)]
				elif len(grid[or_i][or_j]) > 0:
					origin = grid[or_i][or_j][0]
				else:
					continue

				if len(grid[de_i][de_j]) > 1:
					destin = grid[de_i][de_j][random.randint(0, len(grid[de_i][de_j])-1)]
				elif len(grid[de_i][de_j]) > 0:
					destin = grid[de_i][de_j][0]
				else:
					continue

				paths  = utils.graph_utils.k_shortest_paths(
					road_network, origin, destin, 4, weight='length'
				) 

				paths = [[node for node in path if node in existing_stops] for path in paths]
				route_pool.extend(paths)

if __name__ == '__main__':
	existing_routes = utils.json_utils.read_json_object(configs.BUS_ROUTES)
	stop_points     = utils.json_utils.read_json_object(configs.BUS_STOPS)
	road_points     = utils.json_utils.read_json_object(configs.NETWORK)
	road_network    = utils.json_utils.read_network_json(configs.NETWORK)

	existing_stops  = [item['stop_id'] for item in stop_points]

	assert_limits(existing_routes)
	assert_coordinate_limits(road_points)
	
	grid = [[[] for _ in range(DIVISIONS)] for _ in range(DIVISIONS)]

	for stop_item in stop_points:
		i, j = coord_to_square(stop_item['point'][0], stop_item['point'][1])
		grid[i][j].append(stop_item['stop_id'])

	route_pool = [item['stops'] for item in existing_routes]

	print(len(route_pool))

	add_k_shortest()

	print(len(route_pool))
