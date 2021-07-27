import utils.json_utils

if __name__ == '__main__':
	json  = utils.json_utils.read_json_object("../data/json/clustered_routes_stop_sequence.json")
	stops = []
	for obj in json:
		for stop in obj['stops']:
			if stop not in stops:
				stops.append(stop)

	print(f'Existing CARRIS Network covers {len(stops)} stops')
