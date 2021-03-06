
import utils.json_utils
import networkx as nx
import math
import numpy as np

def generate_east_west_routes():
	global grid_size

	terminal_nodes = [
		2000 + ((grid_size-1)*(10**(grid_size//10+1))) + j
		for j in range(grid_size)
	]
	routes = []

	i = 0
	for j in range(grid_size):
		starting_stop = 2000 + (i*(10**(grid_size//10+1))) + j

		route = []
		stop = starting_stop
		i_, j_ = i, j
		while stop not in terminal_nodes:
			route.append(stop)
			possible = ["ahead"]
			if j_ > 0:
				possible.append("down")
			if j_ < grid_size-1:
				possible.append("up")

			direction = None
			if len(possible) == 2:
				direction = np.random.choice(possible, p=[0.75, 0.25])
			elif len(possible) == 3:
				direction = np.random.choice(possible, p=[0.5, 0.25, 0.25])

			if direction == "ahead":
				i_ += 1
			elif direction == "down":
				j_ -= 1
			elif direction == "up":
				j_ += 1

			stop = 2000 + (i_*(10**(grid_size//10+1))) + j_

		route.append(stop)
		routes.append(filter_zags(route))

	return routes


def generate_west_east_routes():
	global grid_size

	terminal_nodes = [
		2000 + ((0)*(10**(grid_size//10+1))) + j
		for j in range(grid_size)
	]
	routes = []

	i = grid_size-1
	for j in range(grid_size):
		starting_stop = 2000 + (i*(10**(grid_size//10+1))) + j

		route = []
		stop = starting_stop
		i_, j_ = i, j
		while stop not in terminal_nodes:
			route.append(stop)
			possible = ["ahead"]
			if j_ > 0:
				possible.append("down")
			if j_ < grid_size-1:
				possible.append("up")

			direction = None
			if len(possible) == 2:
				direction = np.random.choice(possible, p=[0.75, 0.25])
			elif len(possible) == 3:
				direction = np.random.choice(possible, p=[0.5, 0.25, 0.25])

			if direction == "ahead":
				i_ -= 1
			elif direction == "down":
				j_ -= 1
			elif direction == "up":
				j_ += 1

			stop = 2000 + (i_*(10**(grid_size//10+1))) + j_

		route.append(stop)
		routes.append(filter_zags(route))

	return routes


def generate_north_south_routes():
	global grid_size

	terminal_nodes = [
		2000 + ((i)*(10**(grid_size//10+1))) + 0
		for i in range(grid_size)
	]
	routes = []

	j = grid_size-1
	for i in range(grid_size):
		starting_stop = 2000 + (i*(10**(grid_size//10+1))) + j

		route = []
		stop = starting_stop
		i_, j_ = i, j
		while stop not in terminal_nodes:
			route.append(stop)
			possible = ["ahead"]
			if i_ > 0:
				possible.append("left")
			if i_ < grid_size-1:
				possible.append("right")

			direction = None
			if len(possible) == 2:
				direction = np.random.choice(possible, p=[0.75, 0.25])
			elif len(possible) == 3:
				direction = np.random.choice(possible, p=[0.5, 0.25, 0.25])

			if direction == "ahead":
				j_ -= 1
			elif direction == "left":
				i_ -= 1
			elif direction == "right":
				i_ += 1

			stop = 2000 + (i_*(10**(grid_size//10+1))) + j_

		route.append(stop)
		routes.append(filter_zags(route))

	return routes


def generate_south_north_routes():
	global grid_size

	terminal_nodes = [
		2000 + ((i)*(10**(grid_size//10+1))) + (grid_size-1)
		for i in range(grid_size)
	]
	routes = []

	j = 0
	for i in range(grid_size):
		starting_stop = 2000 + (i*(10**(grid_size//10+1))) + j

		route = []
		stop = starting_stop
		i_, j_ = i, j
		while stop not in terminal_nodes:
			route.append(stop)
			possible = ["ahead"]
			if i_ > 0:
				possible.append("left")
			if i_ < grid_size-1:
				possible.append("right")

			direction = None
			if len(possible) == 2:
				direction = np.random.choice(possible, p=[0.75, 0.25])
			elif len(possible) == 3:
				direction = np.random.choice(possible, p=[0.5, 0.25, 0.25])

			if direction == "ahead":
				j_ += 1
			elif direction == "left":
				i_ -= 1
			elif direction == "right":
				i_ += 1

			stop = 2000 + (i_*(10**(grid_size//10+1))) + j_

		route.append(stop)
		routes.append(filter_zags(route))

	return routes


def filter_zags(route):
	changed = True

	while changed:
		changed = False

		for i in range(len(route)-2):
			inspect = route[i:i+3]
			if inspect[0] == inspect[-1]:
				changed = True
				route   = route[0:i] + route[i+2:]
				break 

	return route
				

def routes_to_geojson(routes):
	global bus_stops
	
	lines = []

	for route in routes:
		line = []
		for stop in route:
			line.append([
				item['point'] 
				for item in bus_stops 
				if item['stop_id']==stop
			][0])
		lines.append(line)
	
	return lines


if __name__ == '__main__':
	road_network = utils.json_utils.read_network_json('../data/json/generated_road_network.json')
	bus_stops    = utils.json_utils.read_json_object('../data/json/generated_bus_stops.json')
	stop_ids     = [item['stop_id'] for item in bus_stops]

	grid_size = int(math.sqrt(len(stop_ids)))

	print(f"Grid Size: {grid_size}")

	routes = []

	east_west = generate_east_west_routes()
	routes.extend(east_west)
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_east_west_made.geojson',
		routes_to_geojson(east_west)
	)

	west_east = generate_west_east_routes()
	routes.extend(west_east)
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_west_east_made.geojson',
		routes_to_geojson(west_east)
	)

	north_south = generate_north_south_routes()
	routes.extend(north_south)
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_north_south_made.geojson',
		routes_to_geojson(north_south)
	)

	south_north = generate_south_north_routes()
	routes.extend(south_north)
	utils.json_utils.write_geojson_lines(
		'../data/geojson/generated_south_north_made.geojson',
		routes_to_geojson(south_north)
	)

	assert(len(routes) == grid_size*4)

	for r in routes:
		print(*r)

	utils.json_utils.write_json_object('../data/json/generated_route_pool.json', routes)