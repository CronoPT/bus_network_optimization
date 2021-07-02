import pandas as pd
import numpy as np
import math
import networkx as nx
import utils.general_utils
import utils.geometric_utils
import utils.json_utils
import configs

if __name__ == '__main__':

	points = utils.json_utils.read_json_object(configs.WALKS)
	replacements = utils.json_utils.read_json_object(configs.STOP_REPLACEMENTS)

	walk_points = []
	walk_lines  = []
	mappings = {}

	for id, obj in points.items():
		walk_points.append(obj['point'])
		mappings[int(id)] = obj['point']

	for id, obj in points.items():
		for bus_item in obj['close_bus']:
			if bus_item['stop_id'] in mappings:
				walk_lines.append([
					mappings[int(id)],
					mappings[bus_item['stop_id']]
				])

		for metro_item in obj['close_metro']:
			if metro_item['station_id'] in mappings:
				walk_lines.append([
					mappings[int(id)],
					mappings[metro_item['station_id']]
				])


	utils.json_utils.write_geojson_lines(configs.WALKS_LINES, walk_lines)
	utils.json_utils.write_geojson_points(configs.WALKS_POINTS, walk_points)
