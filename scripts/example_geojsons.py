import utils.json_utils


if __name__ == '__main__':
	traversal_indexes = [3, 9]
	json_traversal = utils.json_utils.read_json_object('../data/geojson/south_north.geojson')
	json_traversal['coordinates'] = [json_traversal['coordinates'][i] for i in traversal_indexes]
	utils.json_utils.write_json_object('../data/geojson/examples/example_generated_traversal.geojson', json_traversal)

	hubs_indexes = [50, 200]
	json_hubs = utils.json_utils.read_json_object('../data/geojson/main_hubs.geojson')
	json_hubs['coordinates'] = [json_hubs['coordinates'][i] for i in hubs_indexes]
	utils.json_utils.write_json_object('../data/geojson/examples/example_generated_hubs.geojson', json_hubs)

	carris_indexes = [80, 120]
	json_carris = utils.json_utils.read_json_object('../data/geojson/clustered_route_shapes.geojson')
	json_carris['coordinates'] = [json_carris['coordinates'][i] for i in carris_indexes]
	utils.json_utils.write_json_object('../data/geojson/examples/example_carris.geojson', json_carris)