
import utils.json_utils
import utils.graph_utils
import utils.general_utils
import utils.geometric_utils
import configs
import numpy as np
import math
import random 
import networkx as nx

MIN_ROUTE_LEN = np.inf 
MAX_ROUTE_LEN = -np.inf
DIVISIONS = 30

max_lat = -np.inf
min_lat = np.inf
max_lon = -np.inf
min_lon = np.inf

lat_step = 0
lon_step = 0

'''
| The top bus stops in terms of
| smartcard validations
'''
main_stops = [
	3802,
	3803,
	917,
	2702,
	9611,
	2712,
	3807,
	801,
	10315,
	5826
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

def coord_to_strip_vertical(lon, divisions):
	'''
	| Similar to coord_to_square but to located
	| a given longitude into a vertical strip
	| in an array of strips with size divisions.
	'''
	global max_lon
	global min_lon

	step = (max_lon-min_lon)/divisions

	x = math.floor( (lon-min_lon) / step )
	x = x if x < divisions else x-1

	return x

def coord_to_strip_horizontal(lat, divisions):
	'''
	| Similar to coord_to_square but to located
	| a given longitude into a horizontal strip
	| in an array of strips with size divisions.
	'''
	global max_lat
	global min_lat

	step = (max_lat-min_lat)/divisions

	y = math.floor( (lat-min_lat) / step )
	y = y if y < divisions else y-1

	return y

def assert_limits(routes):
	'''
	| Find out the minimum and maximum
	| route lenght.
	'''
	global MIN_ROUTE_LEN
	global MAX_ROUTE_LEN

	for route_item in routes:
		route_len = len(route_item['stops'])

		if route_len > MAX_ROUTE_LEN:
			MAX_ROUTE_LEN = route_len
		
		if route_len < MIN_ROUTE_LEN:
			MIN_ROUTE_LEN = route_len

def get_adjacent_squares(i, j, end=False):
	'''
	| Get the neighborign squares of the (i, j)
	| square. If end=False, get the 5x5 centered
	| around (i, j), otherwise get the 3x3 squared
	| centered around (i, j).
	'''
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

	if end:
		return candidate_coords
	else:
		result = []
		for pair in candidate_coords:
			result.extend(get_adjacent_squares(*pair, end=True))
		candidate_coords = [(item[0], item[1]) for item in result]

	return list(set(candidate_coords))


def assert_coordinate_limits(net):
	'''
	| Go over every road node (as everything is
	| contained there) and assess the coordinate
	| limits (lower and upper).
	'''
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

def add_k_shortest_stops():
	'''
	| For the stop stops in terms of validations,
	| compute all the shortest paths between them.
	| The rationale behind this, is that some of those
	| paths, will be useful to connect big hubs.
	|
	| It was supposed to be using Yen's algorithm, but
	| it is taking too long for such big graphs so we
	| loose a bit of route diversity for speed.
	'''
	global route_pool
	global road_network
	global existing_stops

	replacements = utils.json_utils.read_json_object(configs.STOP_REPLACEMENTS)
	routes = []
	for index_out, origin in enumerate(main_stops):
		for index_in, destin in enumerate(main_stops):
			if origin != destin:
				path = None

				origin_ = origin if str(origin) not in replacements else replacements[str(origin)]
				destin_ = destin if str(destin) not in replacements else replacements[str(destin)]

				try:	
					path = nx.algorithms.shortest_paths.weighted.dijkstra_path(
						road_network, 
						source=origin_, 
						target=destin_,
						weight='length'
					)
				except nx.NetworkXNoPath:
					continue

				path = [node for node in path if node in existing_stops]
				route_pool.append(path)
				routes.append(path)

			progress = index_out*len(main_stops) + index_in + 1
			total = len(main_stops)**2
			utils.general_utils.print_progress_bar(progress, total)
			
	route_shapes = []
	for route in routes:
		route_shapes.append(coord_sequence_from_route(route))
	utils.json_utils.write_geojson_lines('../data/geojson/main_hubs.geojson', route_shapes)

def north_south(stop_north, stop_south):
	'''
	| Check if the second stop is
	| SOUTH of the first.
	'''
	return stop_north[1] > stop_south[1]


def south_north(stop_south, stop_north):
	'''
	| Check if the second stop is
	| NORTH of the first.
	'''
	return stop_north[1] > stop_south[1]


def west_east(stop_west, stop_east):
	'''
	| Check if the second stop is
	| EAST of the first.
	'''
	return stop_west[0] < stop_east[0] 


def east_west(stop_east, stop_west):
	'''
	| Check if the second stop is
	| WEST of the first.
	'''
	return stop_west[0] < stop_east[0] 



# How many routes to generate in each direction & way
ROUTES_PER_DIRECTION = 20


def decide_starting_points_north():
	"""
	| Devide all the points in vertical strips, then
	| for each strip, get the points farthest to the
	| North.
	"""
	stops = utils.json_utils.read_json_object(configs.BUS_STOPS)

	strips = [[] for _ in range(ROUTES_PER_DIRECTION)]

	for stop in stops:
		i = coord_to_strip_vertical(stop['point'][0], ROUTES_PER_DIRECTION)
		strips[i].append(stop)

	for strip in strips:
		strip.sort(key=lambda item : item['point'][1], reverse=True)

	return [strip[0]['stop_id'] for strip in strips if len(strip) > 0]


def decide_starting_points_south():
	"""
	| Devide all the points in vertical strips, then
	| for each strip, get the points farthest to the
	| South.
	"""
	stops = utils.json_utils.read_json_object(configs.BUS_STOPS)

	strips = [[] for _ in range(ROUTES_PER_DIRECTION)]

	for stop in stops:
		i = coord_to_strip_vertical(stop['point'][0], ROUTES_PER_DIRECTION)
		strips[i].append(stop)

	for strip in strips:
		strip.sort(key=lambda item : item['point'][1])

	return [strip[0]['stop_id'] for strip in strips if len(strip) > 0]


def decide_starting_points_east():
	"""
	| Devide all the points in horizontal strips, then
	| for each strip, get the points farthest to the
	| East.
	"""
	stops = utils.json_utils.read_json_object(configs.BUS_STOPS)

	strips = [[] for _ in range(ROUTES_PER_DIRECTION)]

	for stop in stops:
		i = coord_to_strip_horizontal(stop['point'][1], ROUTES_PER_DIRECTION)
		strips[i].append(stop)

	for strip in strips:
		strip.sort(key=lambda item : item['point'][0], reverse=True)

	return [strip[0]['stop_id'] for strip in strips if len(strip) > 0]


def decide_starting_points_west():
	"""
	| Devide all the points in horizontal strips, then
	| for each strip, get the points farthest to the
	| West.
	"""
	stops = utils.json_utils.read_json_object(configs.BUS_STOPS)

	strips = [[] for _ in range(ROUTES_PER_DIRECTION)]

	for stop in stops:
		i = coord_to_strip_horizontal(stop['point'][1], ROUTES_PER_DIRECTION)
		strips[i].append(stop)

	for strip in strips:
		strip.sort(key=lambda item : item['point'][0])

	return [strip[0]['stop_id'] for strip in strips if len(strip) > 0]


def generate_directional_routes(start, comparator):
	'''
	| Generate routes in a given direction. They will
	| always have the maximum route length because they
	| can be truncated in the genetic algorithms later
	| down the line.
	| 
	| start: a function to return a list of starting nodes
	| comparator: a function to validate a sequence of stops.
	'''
	global road_network


	stops = utils.json_utils.read_json_object(configs.BUS_STOPS)
	stops_dict = {item['stop_id']:item['point'] for item in stops}
	
	# get the starting point for each route
	starting_points = start()

	routes = []
	for start in starting_points:
		curr_stop = start
		route = [curr_stop]
		for _ in range(MAX_ROUTE_LEN):
			curr_point = stops_dict[curr_stop]
			i, j = coord_to_square(curr_point[0], curr_point[1])
			adjs = get_adjacent_squares(i, j)
			chosen = False
			radius = 2000
			while not chosen:
				c_stops = []
				[c_stops.extend(grid[i_][j_]) for i_, j_ in adjs]
				
				# For the stops in the adjacent squares (5x5 square)
				# centered around (i, j), filter the ones that are
				# withing valid range. 
				c_stops = [stop for stop in c_stops if
					utils.geometric_utils.haversine_distance(
						stops_dict[stop], curr_point
					) < radius
				]

				# For the stops that are in range, filter the ones
				# that are correctly positioned relative to the 
				# current stop.
				c_stops = [stop for stop in c_stops if
					comparator(curr_point, stops_dict[stop])
				]
				
				dists = []
				# Compute the distances on the road between
				# the current stop and all the candidates to
				# be the next stop in the route.
				for stop in c_stops:
					distance = 0
					try:
						distance = nx.algorithms.shortest_paths.weighted.dijkstra_path_length(
							road_network, 
							source=curr_stop, 
							target=stop,
							weight='length'
						)
					except nx.NetworkXNoPath:
						distance = np.inf
					
					if distance != np.inf:
						dists.append((stop, distance))
				
				# Sort candidates by distance to the current stop
				# through the road network.
				dists.sort(key = lambda x : x[1])

				if len(dists) > 0:
					chosen = True
					# Always try to go to the stop which is 
					# closest in terms of road distance.
					curr_stop = dists[0][0]
					route.append(stop)
					break
				else:
					radius += 2000
					
					# No eligeable stops in a reasonable radius,
					# drop this route.
					if radius > 4000:
						break
		
		# It is possible that routes do not have the maximum
		# size because there were no stops satisfying the 
		# criteria. If it is to small, drop it.
		if len(route) > MIN_ROUTE_LEN:
			routes.append(route)

		route_pool.extend(routes)

	return routes


def generate_traversal_routes():
	"""
	| Generate "vertical" and "horizontal"
	| routes in both ways, that is, routes
	| going from North to South and vice-versa
	| and then routes from West to East and
	| vice-versa.
	"""

	# North -> South
	routes = generate_directional_routes(
		decide_starting_points_north, north_south
	)
	route_shapes = []
	for route in routes:
		route_shapes.append(coord_sequence_from_route(route))
	utils.json_utils.write_geojson_lines('../data/geojson/north_south.geojson', route_shapes)

	# South -> North
	routes = generate_directional_routes(
		decide_starting_points_south, south_north
	)
	route_shapes = []
	for route in routes:
		route_shapes.append(coord_sequence_from_route(route))
	utils.json_utils.write_geojson_lines('../data/geojson/south_north.geojson', route_shapes)

	# West -> East
	routes = generate_directional_routes(
		decide_starting_points_west, west_east
	)
	route_shapes = []
	for route in routes:
		route_shapes.append(coord_sequence_from_route(route))
	utils.json_utils.write_geojson_lines('../data/geojson/west_east.geojson', route_shapes)

	# East -> West
	routes = generate_directional_routes(
		decide_starting_points_east, east_west
	)
	route_shapes = []
	for route in routes:
		route_shapes.append(coord_sequence_from_route(route))
	utils.json_utils.write_geojson_lines('../data/geojson/east_west.geojson', route_shapes)


def coord_sequence_from_route(route):
	'''
	| Get the route geometry given a sequence of 
	| stops (a route).
	'''
	global road_network

	start_node = route[0]
	geo = [[road_network.nodes[start_node]['x'],
	        road_network.nodes[start_node]['y']]]
	origin = None
	for destin in route:
		if origin != None:
			path = nx.algorithms.shortest_paths.weighted.dijkstra_path(
				road_network, 
				source=origin, 
				target=destin,
				weight='length'
			)

			origin_node = None
			for destin_node in path:
				if origin_node != None:
					edge_data = road_network.get_edge_data(origin_node, destin_node)

					min_length = np.inf
					min_edge  = None

					for _, edge in edge_data.items():
						if edge['length'] < min_length:
							min_length = edge['length']
							min_edge   = edge

					if 'geometry' in min_edge:
						geo.extend(min_edge['geometry'][1:])
					else:
						geo.append([
							road_network.nodes[destin_node]['x'], 
							road_network.nodes[destin_node]['y']
						])
				origin_node = destin_node
		origin = destin
	return geo


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

	add_k_shortest_stops()
	generate_traversal_routes()
	utils.json_utils.write_json_object(configs.ROUTE_POOL, route_pool)

	print(len(route_pool))
	print(f'max_route_length: {MAX_ROUTE_LEN}')
	print(f'min_route_length: {MIN_ROUTE_LEN}')
