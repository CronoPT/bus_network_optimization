import utils.json_utils
import numpy as np

if __name__ == '__main__':
	walks = utils.json_utils.read_json_object('../data/json/walks.json')

	max_bus = -np.inf
	max_met = -np.inf

	for stop, attr in walks.items():
		if attr['type'] == 'metro':
			if int(stop) > max_met:
				max_met = int(stop) 
		elif attr['type'] == 'bus':
			if int(stop) > max_bus:
				max_bus = int(stop)

	print(f'Highest Bus: {max_bus}')
	print(f'Highest Met: {max_met}')