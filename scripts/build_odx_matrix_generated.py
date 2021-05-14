
import utils.json_utils
import math
import random

if __name__ == '__main__':
	bus_stops    = utils.json_utils.read_json_object('../data/json/generated_bus_stops.json')
	stop_ids     = [item['stop_id'] for item in bus_stops]

	grid_size = int(math.sqrt(len(stop_ids)))

	odx = []
	for origin_i in range(grid_size):
		for origin_j in range(grid_size):
			for destin_i in range(grid_size):
				for destin_j in range(grid_size):
					l = [random.randint(0, 10) for _ in range(5)]
					odx.append({
						'origin': [origin_i, origin_j],
						'destin': [destin_i, destin_j],
						'morning_rush_hour':  l[0],
						'midday': l[1],
						'afternoon_rush_hour': l[2],
						'happy_hour': l[3],
						'night_time': l[4],
						'total': sum(l)
					})

	utils.json_utils.write_json_object('../data/json/generated_odx_matrix.json' ,odx)

