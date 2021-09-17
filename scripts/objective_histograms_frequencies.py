import utils.json_utils
import matplotlib.pyplot as plt
import sys
import numpy as np

indexes   = [0, 12, 24, 36, 49, 62, 74, 99, 124, 149, 174, 199]
lisbon_costs = [
	0.0670429,
	1.13925e+06
]

labels = [
	"Average waiting time (mins)",
	"Total distance travelled (Km)",
]

times = {
	'total': {
		'file': '../data/json/run_frequencies_total.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_total.json',
		'label': 'Full Day',
		'hours': 24,
		'bottom': 0
	},
	'morning': {
		'file': '../data/json/run_frequencies_morning.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_morning.json',
		'label': 'Morning Rush Hour',
		'hours': 3,
		'bottom': 0,
		'color': 'khaki'
	},
	'midday': {
		'file': '../data/json/run_frequencies_midday.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_midday.json',
		'label': 'Middle of Day',
		'hours': 6,
		'bottom': 0,
		'color': 'mediumseagreen'
	},
	'afternoon': {
		'file': '../data/json/run_frequencies_afternoon.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_afternoon.json',
		'label': 'Afternoon Rush Hour',
		'hours': 3,
		'bottom': 0,
		'color': 'lightcoral'
	},
	'happy': {
		'file': '../data/json/run_frequencies_happy.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_happy.json',
		'label': 'Happy Hour',
		'hours': 4,
		'bottom': 0,
		'color': 'mediumpurple'
	},
	'night': {
		'file': '../data/json/run_frequencies_night.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_night.json',
		'label': 'Night Time',
		'hours': 8,
		'bottom': 0,
		'color': 'skyblue'
	}
}

# if __name__ == '__main__':
# 	highlight = None

# 	try:
# 		highlight = int(sys.argv[1])
# 	except ValueError:
# 		highlight = 'naught'

# 	for time, item in times.items():
# 		item['json'] = utils.json_utils.read_json_object(item['file'])

# 	for time, item in times.items():
# 		indexes = [index for index in indexes if index <= len(item['json'])-1]

# 	# for index in indexes:
# 	# 	print(f'n{index} -> {json[index]["costs"][0]} | {json[index]["costs"][1]}')

# 	# print(f'lisbon -> {lisbon_costs[0]} | {lisbon_costs[1]}')
		
# 	# total_km   = 0
# 	# total_wait = 1

# 	fig, axs = plt.subplots(2, 2)

# 	for i in range(2):
# 		x = ["lisbon"]
# 		[x.append(f'f{index}') for index in indexes]
# 		y = [lisbon_costs[i]]
# 		[y.append(times['total']['json'][index]['costs'][i]) for index in indexes]
# 		colors = ['gray']
# 		[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
# 		if i==0:
# 			y = [n*60 for n in y]
# 		axs[0, i].bar(x, y, color=colors)
# 		axs[0, i].set_ylabel(labels[i])
# 		axs[0, i].grid(axis='y')

# 	y = [0 for i in indexes]
# 	y_map = {0:0, 24:1, 49:2, 74:3, 99:4, 124:5, 149:6, 174:7}
# 	y_map = {0:0, 12:1, 24:2, 36:3, 49:4, 62:5, 74:6, 99:4, 124:5, 149:6, 174:7}
# 	for key, value in times.items():
# 		if key != 'total':
# 			for i in indexes:
# 				y[y_map[i]] += times[key]['json'][i]['costs'][0] * (times[key]['hours']/24)

# 	x = ["lisbon"]
# 	[x.append(f'f{index}') for index in indexes]
# 	y = [lisbon_costs[0]] + y
# 	colors = ['gray']
# 	[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
# 	y = [n*60 for n in y]
# 	axs[1, 0].bar(x, y, color=colors)
# 	axs[1, 0].set_ylabel(labels[0])
# 	axs[1, 0].grid(axis='y')


# 	axs[1, 1].bar('lisbon', lisbon_costs[1], color='gray')
# 	bottom = [0 for _ in indexes]
# 	for key, value in times.items():
# 		if key != 'total':
# 			x = ["lisbon"]
# 			x = [f'f{index}' for index in indexes]
# 			y = [times[key]['json'][index]['costs'][1] for index in indexes]
# 			axs[1, 1].bar(x, y, color=times[key]['color'], bottom=bottom, label=times[key]['label'])
# 			bottom = [n+b for b, n in zip(bottom, y)]
	
# 	axs[1, 1].set_ylabel(labels[1])
# 	axs[1, 1].grid(axis='y')
# 	axs[1, 1].legend()

# 	# b = 0
# 	# for key, value in times.items():
# 	# 	if key != 'total':
# 	# 		values = [obj['costs'][1] for obj in value['json']]
# 	# 		m = min(values)
# 	# 		axs[1, 1].bar('hypo', m, bottom=b, color=value['color'])
# 	# 		b += m
# 	# values = [obj['costs'][1] for obj in times['total']['json']]
# 	# m = min(values)
# 	# axs[1, 1].bar('hypo-tot', m, color='black')

# 	# plt.savefig(f'../data/images/network-{highlight}-highlight.png', bbox_inches='tight', dpi=400)
# 	plt.show()

def least_squares_standard(X, t):
	return np.dot(np.dot(np.linalg.inv(np.dot(np.matrix.transpose(X), X)), np.matrix.transpose(X)), t)

if __name__ == '__main__':
	highlight = None

	try:
		highlight = int(sys.argv[1])
	except ValueError:
		highlight = 'naught'

	for time, item in times.items():
		item['json'] = utils.json_utils.read_json_object(item['file'])

	for time, item in times.items():
		indexes = [index for index in indexes if index <= len(item['json'])-1]

	# for index in indexes:
	# 	print(f'n{index} -> {json[index]["costs"][0]} | {json[index]["costs"][1]}')

	# print(f'lisbon -> {lisbon_costs[0]} | {lisbon_costs[1]}')
		
	# total_km   = 0
	# total_wait = 1

	fig, axs = plt.subplots(1, 2)

	for t, time in enumerate(times):
		if t == 0:
			print(f'{t} {time}')
			for i in range(2):
				x = ["lisbon"]
				# x = []
				[x.append(f'f{index}') for index in indexes]
				y = [lisbon_costs[i]]
				# y = []
				[y.append(times[time]['json'][index]['costs'][i]) for index in indexes]
				colors = ['gray']
				[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
				if i==0:
					y = [n*60 for n in y]
				axs[i].bar(x, y, color=colors)
				axs[i].set_ylabel(labels[i])
				axs[i].grid(axis='y')
	plt.show()

	for key, value in times.items():
		sets = []
		for index in indexes:
			sets.append({
				'set': f'f{index}',
				'waiting':  value['json'][index]['costs'][0],
				'distance': value['json'][index]['costs'][1],
			})
		sets.sort(key=lambda obj:obj['distance'])

		print()
		print(key)
		for index, obj in enumerate(sets):
			rating = 8-index
			obj["rating"] = rating
			print(f'{obj["set"]} |\t waiting: {obj["waiting"]} | distance: {obj["distance"]} | rating: {obj["rating"]}')

		value["picks"] = sets

	for key, value in times.items():
		value["picks"].sort(key=lambda obj: obj['set'])
		X = np.array([[1, obj["waiting"], obj["distance"]] for obj in value["picks"]])
		t = np.array([10-obj["rating"] for obj in value["picks"]])

		if key == 'total':
			t = np.array([8, 3, 3, 8, 4, 5, 6])
		elif key == 'morning':
			t = np.array([1, 5, 7, 5, 3, 5, 5])

		print(X)
		print(t)

		weights = least_squares_standard(X, t)

		print(f'{key}: {weights}')

	for key, value in times.items():
		json = utils.json_utils.read_json_object(value["summary_file"])
		waits = []
		distances = []
		for iteration in json["summary"]:
			waits.extend([obj[0] for obj in iteration])
			distances.extend([obj[1] for obj in iteration])
		print(f'{key} -> wait_max: {max(waits)} | wait_min: {min(waits)} | distance_max: {max(distances)} | distance_min: {min(distances)}')