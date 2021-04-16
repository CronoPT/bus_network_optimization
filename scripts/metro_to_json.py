
import configs
import utils.json_utils
import utils.geometric_utils
import networkx as nx

def int_metro_id(metro_id):
	return int(metro_id.split('M')[1])

if __name__ == '__main__':
	metro_stations = utils.json_utils.read_json_object(configs.METRO_STATIONS)
	metro_lines    = utils.json_utils.read_json_object(configs.METRO_NETWORK)
	metro_network  = nx.DiGraph()

	for station_id, station_props in metro_stations.items():
		metro_network.add_node(int_metro_id(station_id), **{
			'x': station_props['point'][0],
			'y': station_props['point'][1],
			'name': station_props['nome']
		})

	for color, line in metro_lines.items():
		origin = None
		for destin in line:
			if origin != None:
				distance = utils.geometric_utils.haversine_distance(
					metro_stations[origin]['point'],
					metro_stations[destin]['point']
				)
				time = (distance / (configs.METRO_SPEED / 3.6))
				metro_network.add_edge(
					int_metro_id(origin), 
					int_metro_id(destin),
					**{
						'origin_id' : int_metro_id(origin),
						'destin_id' : int_metro_id(destin),
						'line_color': color,
						'time': time
					}
				)
				metro_network.add_edge(
					int_metro_id(destin), 
					int_metro_id(origin),
					**{
						'origin_id' : int_metro_id(destin),
						'destin_id' : int_metro_id(origin),
						'line_color': color,
						'time': time
					}
				)
			origin = destin

	utils.json_utils.write_networkx_json(configs.METRO_NETWORKNX, metro_network)		

