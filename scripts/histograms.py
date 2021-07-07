
import utils.json_utils
import matplotlib.pyplot as plt
import random 

def do_transfers(json):
	transfers   = {}
	for obj in json:
		# t = obj['original_transfers'] - obj['generate_transfers']
		t = random.randint(-3, 3)
		if t not in transfers:
			transfers[t] = 0
		transfers[t] += obj['passengers']

	colors = []
	for key in transfers:
		if key == 0:
			colors.append('gray')
		elif key < 0:
			colors.append('green')
		else:
			colors.append('red')

	plt.bar(transfers.keys(), transfers.values(), color=colors, width=0.95)
	plt.show()

def do_travel_time(json):
	times = {i:0 for i in range(-60, 65, 5)}
	ticks = [i for i in range(-60, 65, 5)]
	for obj in json:
		# t = obj['original_travel_time'] - obj['generate_travel_time']
		t = random.randint(-60, 60)
		i = (t // 5) * 5
		times[i] += obj['passengers']

	colors = []
	for key in times:
		if key == 0:
			colors.append('gray')
		elif key < 0:
			colors.append('green')
		else:
			colors.append('red')

	plt.bar(times.keys(), times.values(), color=colors, width=4.8)
	plt.xticks(ticks)
	plt.show()
			
if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/comparison.json')

	do_transfers(json)
	do_travel_time(json)