
import utils.json_utils

lisbon_costs = [
	3.56122e+06, # total length (in meters)
	0.00698877, # unsatisfied demand 
	2.49523e+08, # in vehicle time (in seconds)
	0.187626, # average transfers ()
]
OBJECTIVES = 4
names = {
	0: 'total_length',
	1: 'unsatisfied_demand',
	2: 'in_vehicle_time',
	3: 'average_transfers'
}

if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/run_nets.json')

	min_maxs = []

	for o in range(OBJECTIVES):
		objectives = [obj['costs'][o] for obj in json]
		objectives.append(lisbon_costs[o])

		high = max(objectives)
		low  = min(objectives)

		min_maxs.append({
			'max': high,
			'min': low
		})

	for obj in json:
		new_costs = []
		for index, objective in enumerate(obj['costs']):
			new_costs.append((objective - min_maxs[index]['min']) / (min_maxs[index]['max'] - min_maxs[index]['min']))
		print(new_costs)

	


