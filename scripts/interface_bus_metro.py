import configs
import utils.json_utils
import utils.geometric_utils
import utils.general_utils
import json

def int_metro_id(metro_id):
	return int(f"11111{metro_id.split('M')[1]}")

if __name__ == '__main__':
	road_network   = utils.json_utils.read_network_json(configs.NETWORK)
	metro_stations = utils.json_utils.read_json_object(configs.METRO_STATIONS)
	bus_stops = utils.json_utils.read_json_object(configs.BUS_STOPS) 

	print(f'Old edges {road_network.number_of_edges()}')
	print(f'Old nodes {road_network.number_of_nodes()}')

	print(json.dumps(metro_stations, indent=4))

	for metro_id, metro_station in metro_stations.items():
		print(f"{metro_id} -> {metro_station}")
		metro_id_int = int_metro_id(metro_id)
		road_network.add_node(metro_id_int, **{
			'x':  metro_station['point'][0],
			'y':  metro_station['point'][1],
		})
		# print(f'Adding metro node {metro_id_int}')

		for bus_stop in bus_stops:
			distance = utils.geometric_utils.haversine_distance(
				metro_station['point'], bus_stop['point']
			)
			if distance < configs.MAX_DIST_METRO_BUS:
				bus_stop_node = road_network.nodes[bus_stop['stop_id']]

				if 'close_bus' not in metro_station:
					metro_station['close_bus'] = []
				if 'close_metro' not in bus_stop:
					bus_stop['close_metro'] = []
				if 'close_metro' not in bus_stop_node:
					bus_stop_node['close_metro'] = []

				metro_station['close_bus'].append(bus_stop['stop_id'])
				bus_stop['close_metro'].append(metro_id_int)
				bus_stop_node['close_metro'].append(metro_id_int)

				road_network.add_edge(bus_stop['stop_id'], metro_id_int, **{
					'walking': True,
					'length':  distance
				})

				road_network.add_edge(metro_id_int, bus_stop['stop_id'], **{
					'walking': True,
					'length':  distance
				})

	blue   = configs.BLUE_LINE
	yellow = configs.YELLOW_LINE
	green  = configs.GREEN_LINE
	red    = configs.RED_LINE

	metro_lines = [blue, yellow, green, red]

	for metro_line in metro_lines:
		s1 = None
		for s2 in metro_line:
			print(s1, s2)
			if s1 != None:
				s1_int = int_metro_id(s1)
				s2_int = int_metro_id(s2)
				distance = utils.geometric_utils.haversine_distance(
					metro_stations[s1]['point'],
					metro_stations[s2]['point']
				)

				road_network.add_edge(s1_int, s2_int, **{
					'metro':  True,
					'length': distance
				})

				road_network.add_edge(s2_int, s1_int, **{
					'metro':  True,
					'length': distance
				})
			s1 = s2

	print(f'New edges {road_network.number_of_edges()}')
	print(f'New nodes {road_network.number_of_nodes()}')

	utils.json_utils.write_networkx_json(configs.NETWORK_WITH_METRO, road_network)
	utils.json_utils.write_json_object(configs.INTERFACED_METRO, metro_stations)
	utils.json_utils.write_json_object(configs.INTERFACED_BUS, bus_stops)

