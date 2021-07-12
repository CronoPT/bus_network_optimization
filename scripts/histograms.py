
import utils.json_utils
import matplotlib.pyplot as plt
import random 

def do_transfers(json):
	transfers   = {}
	for obj in json:
		t = obj['original_transfers'] - obj['generate_transfers']
		# t = random.randint(-3, 3)
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

	plt.figure(figsize=(6, 6))
	plt.bar(transfers.keys(), transfers.values(), color=colors, width=0.95)
	plt.ylabel('Passenger Count')
	plt.xlabel('Transfer difference from the original network (m)')
	plt.savefig(f'../data/images/network-{0}-transfers.png', bbox_inches='tight', dpi=400)
	plt.show()

def do_travel_time(json):
	times = {i:0 for i in range(-60, 65, 5)}
	ticks = [i for i in range(-60, 65, 5)]
	for obj in json:
		t = obj['original_travel_time'] - obj['generate_travel_time']
		# t = random.randint(-60, 60)
		i = (t // 5) * 5
		if i < -60:
			i = -60
		elif i > 60:
			i = 60
		times[i] += obj['passengers']

	colors = []
	for key in times:
		if key == 0:
			colors.append('gray')
		elif key < 0:
			colors.append('green')
		else:
			colors.append('red')

	plt.figure(figsize=(16, 6))
	plt.bar(times.keys(), times.values(), color=colors, width=4.8)
	plt.ylabel('Passenger Count')
	plt.xlabel('Travel time difference from the original network (m)')
	plt.xticks(ticks)
	plt.savefig(f'../data/images/network-{0}-time.png', bbox_inches='tight', dpi=400)
	plt.show()
			
if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/comparison.json')

	do_transfers(json)
	do_travel_time(json)