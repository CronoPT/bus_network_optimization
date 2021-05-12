
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
			if j > 0:
				possible.append("down")
			if j < grid_size-1:
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
			print(stop)

		routes.append(route)

	return routes


if __name__ == '__main__':
	road_network = utils.json_utils.read_network_json('../data/json/generated_road_network.json')
	bus_stops    = utils.json_utils.read_json_object('../data/json/generated_bus_stops.json')
	stop_ids     = [item['stop_id'] for item in bus_stops]

	grid_size = int(math.sqrt(len(stop_ids)))

	print(f"Grid Size: {grid_size}")

	generate_east_west_routes()