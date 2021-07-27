from numpy import sin
import utils.json_utils
import matplotlib.pyplot as plt
import numpy as np
import math
import seaborn as sns

INDEX = 0

# dict: {thing:count}
def series_from_dict(dict):
	series = []
	for thing, count in dict.items():
		[series.append(thing) for i in range(math.ceil(count))]
	return series

def do_transport_modes(json, network):
	modes  = {1:0, 2:0, 3:0}
	single = {"walk":0, "metro":0, "bus":0}
	for obj in json:
		t = obj[f'{network}_modes']
		if t == 1:
			if obj[f'{network}_uses_walking']:
				single["walk"] += obj["passengers"]
			elif obj[f'{network}_uses_metro']:
				single["metro"] += obj["passengers"]
			elif obj[f'{network}_uses_bus']:
				single["bus"] += obj["passengers"]
		else:
			modes[t] += obj['passengers']

	plt.figure(figsize=(6, 6))
	plt.bar(modes.keys(), modes.values(), width=0.95, label="combined modes")
	plt.bar([1], [single["walk"]], width=0.95, label="walk")
	plt.bar([1], [single["metro"]], width=0.95, label="metro", bottom=[single["walk"]])
	plt.bar([1], [single["bus"]], width=0.95, label="bus", bottom=[single["walk"]+single["metro"]])
	plt.xticks([n for n in modes.keys() if n%1 == 0])
	plt.ylabel('Passenger Count')
	plt.xlabel('Number of transport modes used')
	plt.legend()
	plt.show()

def do_walking_time(json, network):
	max = -np.inf
	min = np.inf
	for obj in json:
		t = obj[f'{network}_walking_time']/60 #divide by 60 for because it is in seconds
		i = (t // 5) * 5
		if t < min:
			min = t
		if t > max:
			max = t

	times = {i:0 for i in range(int(min), int(max)+5, 5)}
	ticks = [i for i in range(int(min), int(max)+5, 5)]
	for obj in json:
		t = obj[f'{network}_walking_time']/60 #divide by 60 for because it is in seconds
		i = (t // 5) * 5
		if i > 60:
			i = 60
		times[i] += obj['passengers']

	plt.figure(figsize=(16, 6))
	plt.bar(times.keys(), times.values(), width=5, align='edge', edgecolor='black')
	plt.ylabel('Passenger Count')
	plt.xlabel('Walking Time (mins)')
	plt.xticks(ticks)
	plt.show()

def do_transfers(json, network):
	transfers = {0:0, 1:0, 2:0, 3:0}
	ticks = [0, 1, 2, 3]
	for obj in json:
		t = obj[f'{network}_transfers']
		transfers[t] += obj['passengers']

	plt.bar(transfers.keys(), transfers.values(), width=0.98)
	plt.ylabel('Passenger Count')
	plt.xlabel('Number of transfers')
	plt.xticks(ticks)
	plt.show()

def do_travel_time(json, network):
	max = -np.inf
	min = np.inf
	for obj in json:
		t = obj[f'{network}_travel_time']/60 #divide by 60 for because it is in seconds
		i = (t // 5) * 5
		if i < min:
			min = i
		if i > max:
			max = i

	times = {i:0 for i in range(int(min), int(max)+5, 5)}
	ticks = [i for i in range(int(min), int(max)+5, 5)]
	for obj in json:
		t = obj[f'{network}_travel_time']/60 #divide by 60 for because it is in seconds
		i = (t // 5) * 5
		if i > 60:
			i = 60
		times[i] += obj['passengers']

	print(times)
	print(f'max->{max}')
	print(f'min->{min}')

	plt.figure(figsize=(16, 6))
	plt.bar(times.keys(), times.values(), width=5, align='edge', edgecolor='black')
	plt.ylabel('Passenger Count')
	plt.xlabel('Total travel time (mins)')
	plt.xticks(ticks)
	plt.show()

if __name__ == '__main__':
	json = utils.json_utils.read_json_object(f'../data/json/comparisons/comparison-{INDEX}.json')
	plt.rcParams['font.size'] = '12'

	network = 'original'

	do_walking_time(json, network)
	do_transport_modes(json, network)
	do_transfers(json, network)
	do_travel_time(json, network)