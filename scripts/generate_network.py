
import utils.json_utils
import utils.geometric_utils
import numpy as np
import configs
import math
import networkx as nx

_DIVISIONS  = 10
_ROAD_BASE  = 0
_BUS_BASE   = 10000
_METRO_BASE = 100000000

def coord_to_square(lon, lat):
	'''
	| This function maps a coordinate pair to 
	| a x&y position in the greed.
	| x indexes longitude and y indexes latitude.
	'''

	global lon_step
	global lat_step

	x = math.floor( (lon-min_lon) / lon_step )
	y = math.floor( (lat-min_lat) / lat_step )
	x = x if x < _DIVISIONS else x-1
	y = y if y < _DIVISIONS else y-1
	return x, y

class Square:

	road_nodes_count = 0

	def __init__(self, i, j):
		self.i = i
		self.j = j
		self.bus   = _BUS_BASE   + (j*_DIVISIONS) + i
		self.metro = _METRO_BASE + (j*_DIVISIONS) + i
		
		self.left_road  = None
		self.right_road = None
		self.upper_road = None
		self.lower_road = None

		if i == 0:
			self.left_road = Square.road_nodes_count
			Square.road_nodes_count += 1
		if i == _DIVISIONS-1:
			self.right_road = Square.road_nodes_count
			Square.road_nodes_count += 1
		if j == 0:
			self.upper_road = Square.road_nodes_count
			Square.road_nodes_count += 1
		if j == _DIVISIONS-1:
			self.lower_road = Square.road_nodes_count
			Square.road_nodes_count += 1

	def get_center_coordinates(self):
		lon = min_lon + self.i*lon_step + lon_step/2
		lat = min_lat + self.j*lat_step + lat_step/2
		return (lon, lat)

	def get_metro_coordinates(self):
		center = self.get_center_coordinates()
		lon = center[0] + lon_step/4
		lat = center[1] - lat_step/4
		return (lon, lat)

	def get_bus_coordinates(self):
		return self.get_center_coordinates()

	def get_left_road_coordinates(self):
		center = self.get_center_coordinates()
		lon = center[0] - lon_step/2
		lat = center[1]
		return (lon, lat)

	def get_right_road_coordinates(self):
		center = self.get_center_coordinates()
		lon = center[0] + lon_step/2
		lat = center[1]
		return (lon, lat)

	def get_upper_road_coordinates(self):
		center = self.get_center_coordinates()
		lon = center[0] 
		lat = center[1] - lat_step/2
		return (lon, lat)

	def get_lower_road_coordinates(self):
		center = self.get_center_coordinates()
		lon = center[0] 
		lat = center[1] + lat_step/2
		return (lon, lat)

def build_road_network():
	global squares

	bus_stop_locations = []
	bus_routes  = []
	bus_points  = []
	bus_lines   = []
	road_points = []
	road_lines  = []
	road_network = nx.MultiDiGraph()
	
	for column in squares:
		for square in column:
			bus_stop_locations.append({
				'stop_id': square.bus,
				'point'  : square.get_bus_coordinates()
			})

			road_network.add_node(square.bus, **{
				'x': square.get_bus_coordinates()[0],
				'y': square.get_bus_coordinates()[1]
			})

			bus_points.append(square.get_bus_coordinates())
			road_points.append(square.get_bus_coordinates())

			if square.left_road != None:
				road_network.add_node(square.left_road, **{
					'x': square.get_left_road_coordinates()[0],
					'y': square.get_left_road_coordinates()[1],
				})
				distance = utils.geometric_utils.haversine_distance(
					square.get_left_road_coordinates(),
					square.get_bus_coordinates()
				)
				road_network.add_edge(square.bus, square.left_road, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_network.add_edge(square.left_road, square.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_points.append(square.get_left_road_coordinates())
				road_lines.append([
					square.get_bus_coordinates(),
					square.get_left_road_coordinates()
				])

			if square.right_road != None:
				road_network.add_node(square.right_road, **{
					'x': square.get_right_road_coordinates()[0],
					'y': square.get_right_road_coordinates()[1],
				})
				distance = utils.geometric_utils.haversine_distance(
					square.get_right_road_coordinates(),
					square.get_bus_coordinates()
				)
				road_network.add_edge(square.bus, square.right_road, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_network.add_edge(square.right_road, square.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_points.append(square.get_right_road_coordinates())
				road_lines.append([
					square.get_bus_coordinates(),
					square.get_right_road_coordinates()
				])

			if square.upper_road != None:
				road_network.add_node(square.upper_road, **{
					'x': square.get_upper_road_coordinates()[0],
					'y': square.get_upper_road_coordinates()[1],
				})
				distance = utils.geometric_utils.haversine_distance(
					square.get_upper_road_coordinates(),
					square.get_bus_coordinates()
				)
				road_network.add_edge(square.bus, square.upper_road, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_network.add_edge(square.upper_road, square.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_points.append(square.get_upper_road_coordinates())
				road_lines.append([
					square.get_bus_coordinates(),
					square.get_upper_road_coordinates()
				])

			if square.lower_road != None:
				road_network.add_node(square.lower_road, **{
					'x': square.get_lower_road_coordinates()[0],
					'y': square.get_lower_road_coordinates()[1],
				})
				distance = utils.geometric_utils.haversine_distance(
					square.get_lower_road_coordinates(),
					square.get_bus_coordinates()
				)
				road_network.add_edge(square.bus, square.lower_road, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_network.add_edge(square.lower_road, square.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_points.append(square.get_lower_road_coordinates())
				road_lines.append([
					square.get_bus_coordinates(),
					square.get_lower_road_coordinates()
				])

	for column in squares:
		for square in column:
			i = square.i
			j = square.j

			# if the square is not in the right end of the grid, connect
			# the bus stop with the bus stop to the right
			if i != _DIVISIONS-1:
				i_prime = i + 1
				j_prime = j
				square_prime = squares[i_prime][j_prime]
				distance = utils.geometric_utils.haversine_distance(
					square.get_bus_coordinates(),
					square_prime.get_bus_coordinates()
				)
				road_network.add_edge(square.bus, square_prime.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_network.add_edge(square_prime.bus, square.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_lines.append([
					square.get_bus_coordinates(),
					square_prime.get_bus_coordinates()
				])
				bus_lines.append([
					square.get_bus_coordinates(),
					square_prime.get_bus_coordinates()
				])

			if j != _DIVISIONS-1:
				i_prime = i
				j_prime = j + 1
				square_prime = squares[i_prime][j_prime]
				distance = utils.geometric_utils.haversine_distance(
					square.get_bus_coordinates(),
					square_prime.get_bus_coordinates()
				)
				road_network.add_edge(square.bus, square_prime.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_network.add_edge(square_prime.bus, square.bus, **{
					'length'  : distance,
					'maxspeed': '50'
				})
				road_lines.append([
					square.get_bus_coordinates(),
					square_prime.get_bus_coordinates()
				])
				bus_lines.append([
					square.get_bus_coordinates(),
					square_prime.get_bus_coordinates()
				])

	for i in range(_DIVISIONS):
		route = []
		for j in range(_DIVISIONS):
			route.append(squares[i][j].bus)
		bus_routes.append({
			'route_id': str(len(bus_routes)),
			'stops': route
		})

	for j in range(_DIVISIONS):
		route = []
		for i in range(_DIVISIONS):
			route.append(squares[i][j].bus)
		bus_routes.append({
			'route_id': str(len(bus_routes)),
			'stops': route
		})

	utils.json_utils.write_json_object(
		'../data/json/generated_bus_stops.json',
		bus_stop_locations
	)
	utils.json_utils.write_json_object(
		'../data/json/generated_routes_stop_sequences.json',
		bus_routes	
	)
	utils.json_utils.write_networkx_json(
		'../data/json/generated_road_network.json',
		road_network	
	)
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_road_links.geojson',
		road_lines
	)
	utils.json_utils.write_geojson_points(
		'../data/geojson/generated_road_nodes.geojson',
		road_points
	)	
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_bus_lines.geojson',
		bus_lines
	)
	utils.json_utils.write_geojson_points(
		'../data/geojson/generated_bus_stops.geojson',
		bus_points
	)		

def build_metro_network():
	global squares
	
	metro_network = nx.DiGraph()
	metro_points  = []
	metro_lines   = [] 
	for column in squares:
		for square in column:
			metro_network.add_node(square.metro, **{
				'x': square.get_metro_coordinates()[0],
				'y': square.get_metro_coordinates()[1]
			})
			metro_points.append(square.get_metro_coordinates())

	total_lines = 0
	for i in range(_DIVISIONS):
		j = 0
		i_prime = i+1
		j_prime = j+1
		while i_prime<_DIVISIONS and j_prime<_DIVISIONS:
			square_1 = squares[i_prime][j_prime]
			square_2 = squares[i_prime-1][j_prime-1]
			distance = utils.geometric_utils.haversine_distance(
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			)
			time = (distance / (configs.METRO_SPEED / 3.6))
			metro_network.add_edge(square_1.metro, square_2.metro, **{
				'origin_id':  square_1.metro,
				'destin_id':  square_2.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_network.add_edge(square_2.metro, square_1.metro, **{
				'origin_id':  square_2.metro,
				'destin_id':  square_1.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_lines.append([
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			])

			i_prime += 1
			j_prime += 1
		total_lines += 1

	for i in range(_DIVISIONS):
		j = 0
		i_prime = i-1
		j_prime = j+1
		while i_prime>=0 and j_prime<_DIVISIONS:
			square_1 = squares[i_prime][j_prime]
			square_2 = squares[i_prime+1][j_prime-1]
			distance = utils.geometric_utils.haversine_distance(
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			)
			time = (distance / (configs.METRO_SPEED / 3.6))
			metro_network.add_edge(square_1.metro, square_2.metro, **{
				'origin_id':  square_1.metro,
				'destin_id':  square_2.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_network.add_edge(square_2.metro, square_1.metro, **{
				'origin_id':  square_2.metro,
				'destin_id':  square_1.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_lines.append([
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			])

			i_prime -= 1
			j_prime += 1
		total_lines += 1

	for j in range(1, _DIVISIONS, 1):
		i = 0
		i_prime = i+1
		j_prime = j+1
		while i_prime<_DIVISIONS and j_prime<_DIVISIONS:
			square_1 = squares[i_prime][j_prime]
			square_2 = squares[i_prime-1][j_prime-1]
			distance = utils.geometric_utils.haversine_distance(
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			)
			time = (distance / (configs.METRO_SPEED / 3.6))
			metro_network.add_edge(square_1.metro, square_2.metro, **{
				'origin_id':  square_1.metro,
				'destin_id':  square_2.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_network.add_edge(square_2.metro, square_1.metro, **{
				'origin_id':  square_2.metro,
				'destin_id':  square_1.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_lines.append([
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			])

			i_prime += 1
			j_prime += 1
		total_lines += 1

	for j in range(1, _DIVISIONS, 1):
		i = _DIVISIONS-1
		i_prime = i-1
		j_prime = j+1
		while i_prime>=0 and j_prime<_DIVISIONS:
			square_1 = squares[i_prime][j_prime]
			square_2 = squares[i_prime+1][j_prime-1]
			distance = utils.geometric_utils.haversine_distance(
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			)
			time = (distance / (configs.METRO_SPEED / 3.6))
			metro_network.add_edge(square_1.metro, square_2.metro, **{
				'origin_id':  square_1.metro,
				'destin_id':  square_2.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_network.add_edge(square_2.metro, square_1.metro, **{
				'origin_id':  square_2.metro,
				'destin_id':  square_1.metro,
				'line_color': f'line{total_lines}',
				'time':       time
			})

			metro_lines.append([
				square_1.get_metro_coordinates(),
				square_2.get_metro_coordinates()
			])

			i_prime -= 1
			j_prime += 1
		total_lines += 1

	utils.json_utils.write_networkx_json(
		'../data/json/generated_metro_network.json',
		metro_network
	)
	utils.json_utils.write_geojson_points(
		'../data/geojson/generated_metro_stations.geojson',
		metro_points
	)
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_metro_lines.geojson',
		metro_lines
	)

def build_walks():
	global squares

	walks = {}
	for column in squares:
		for square in column:
			distance = utils.geometric_utils.haversine_distance(
				square.get_bus_coordinates(),
				square.get_metro_coordinates()
			)
			time = (distance / (configs.WALKING_SPEED / 3.6))
			walks[str(square.bus)] = {
				'type': 'bus',
				'close_bus': [],
				'close_metro': [{
					'station_id': square.metro,
					'time': time
				}],
				'point': square.get_bus_coordinates()
			}
			walks[str(square.metro)] = {
				'type': 'metro',
				'close_bus': [{
					'stop_id': square.bus,
					'time': time
				}],
				'close_metro': [],
				'point': square.get_metro_coordinates()	
			}

	utils.json_utils.write_json_object('../data/json/generated_walks.json', walks)

def build_grid():
	lines = []
	for i in range(_DIVISIONS+1):
		lines.append([
			[min_lon, min_lat + i*lat_step],
			[max_lon, min_lat + i*lat_step]
		])
		lines.append([
			[min_lon + i*lon_step, min_lat],
			[min_lon + i*lon_step, max_lat]
		])
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_grid.geojson',
		lines
	)

if __name__ == '__main__':
	net = utils.json_utils.read_json_object(configs.NETWORK)

	max_lat = -np.inf
	min_lat = np.inf
	max_lon = -np.inf
	min_lon = np.inf

	# Just assessing the limits of the grid
	for node in net['nodes']:
		lon = node['x']
		lat = node['y']

		max_lat = lat if lat > max_lat else max_lat
		min_lat = lat if lat < min_lat else min_lat
		max_lon = lon if lon > max_lon else max_lon
		min_lon = lon if lon < min_lon else min_lon

	lat_step = (max_lat-min_lat)/_DIVISIONS
	lon_step = (max_lon-min_lon)/_DIVISIONS

	squares = [[Square(i, j) 
		for j in range(_DIVISIONS)] 
		for i in range(_DIVISIONS)]

	print(len(squares))
	[print(len(line)) for line in squares]

	build_road_network()
	print("Road OK")
	build_metro_network()
	print("Metro OK")
	build_walks()
	print("Walk OK")
	build_grid()