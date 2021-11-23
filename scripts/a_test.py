
import utils.json_utils

if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/routes_stop_sequence.json')

	total_stops = 0
	stop_count_map = {}

	for obj in json:
		total_stops += len(obj['stops'])
		for stop in obj['stops']:
			if stop not in stop_count_map:
				stop_count_map[stop] = 0
			stop_count_map[stop] += 1
	total_stops /= len(json)


	total_count = 0
	for stop, count in stop_count_map.items():
		total_count += count
	total_count /= len(stop_count_map)

	print(f'Average stops per route: {total_stops}')
	print(f'Average routes per stop: {total_count}')