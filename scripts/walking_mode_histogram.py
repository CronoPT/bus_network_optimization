from numpy import sin
import utils.json_utils
import matplotlib.pyplot as plt

INDEX = 0

def do_transport_modes(json):
	modes  = {1:0, 2:0, 3:0}
	single = {"walk":0, "metro":0, "bus":0}
	for obj in json:
		t = obj['generate_modes']
		if t == 1:
			if obj['generate_uses_walking']:
				single["walk"] += obj["passengers"]
			elif obj['generate_uses_metro']:
				single["metro"] += obj["passengers"]
			elif obj['generate_uses_bus']:
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

def do_walking_time(json):
	times = {i:0 for i in range(0, 65, 5)}
	ticks = [i for i in range(0, 65, 5)]
	for obj in json:
		t = obj['generate_walking_time']
		i = (t // 5) * 5
		if i > 60:
			i = 60
		times[i] += obj['passengers']

	plt.figure(figsize=(16, 6))
	plt.bar(times.keys(), times.values(), width=4.8)
	plt.ylabel('Passenger Count')
	plt.xlabel('Walking Time (mins)')
	plt.xticks(ticks)
	plt.show()
			
if __name__ == '__main__':
	json = utils.json_utils.read_json_object(f'../data/json/comparisons/comparison-{INDEX}.json')
	plt.rcParams['font.size'] = '12'

	# do_walking_time(json)
	do_transport_modes(json)