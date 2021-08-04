
import utils.json_utils
import matplotlib.pyplot as plt
import random
import numpy as np
import decimal
import math
import sys

def do_transfers(json):
	transfers   = {}
	for obj in json:
		t = obj['generate_transfers'] - obj['original_transfers']
		if t not in transfers:
			transfers[t] = 0
		transfers[t] += obj['passengers']
		if t!=0:
			print(f'{obj["origin"]}->{obj["destin"]}: {obj["original_transfers"]} VS {obj["generate_transfers"]} with {obj["passengers"]} passengers | t={t}')

	# del transfers[0]

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
	plt.xlabel('Transfer difference from the original network')
	plt.xticks([n for n in transfers.keys() if n%1 == 0])
	plt.grid(axis='y')
	plt.savefig(f'../data/images/network-{INDEX}-transfers.png', bbox_inches='tight', dpi=400)
	if SHOW:
		plt.show()
			
def do_travel_time(json):
	max_t = -np.inf
	min_t = np.inf
	max = -np.inf
	min = np.inf

	x_pos = []
	x_neg = []
	x_zero = []
	w_pos = []
	w_neg = []
	w_zero = []
	c = []
	for obj in json:
		t = (obj['generate_travel_time'] - obj['original_travel_time'])/60

		if t > 2.5 or t < -2.5: 
			if t>0:
				x_pos.append(t)
				w_pos.append(obj['passengers'])
			else:
				x_neg.append(t)
				w_neg.append(obj['passengers'])
		else:
			x_zero.append(t)
			w_zero.append(obj['passengers'])
	# 	i = 0

		if t < min_t:
			min_t = t
		if t > max_t:
			max_t = t
		
	# 	if t < 0:
	# 		i = (math.ceil(t / 2.5) * 2.5)
	# 		if i%5!=0:
	# 			i -= 2.5
	# 	elif t > 0:
	# 		i = (math.floor(t / 2.5) * 2.5)
	# 		if i%5!=0:
	# 			i += 2.5
	# 	else:
	# 		i = 0

	# 	if t>0:
	# 		print(f'{t} -> {i} ({(math.floor(t / 2.5) * 2.5)})')
	# 	if t<0:
	# 		print(f'{t} -> {i} ({(math.ceil(t / 2.5) * 2.5)})')

	# 	if i < min:
	# 		min = i
	# 	if i > max:
	# 		max = i

	# print(f'max_t -> {max_t}')
	# print(f'min_t -> {min_t}')
	# print(f'max i -> {max}')
	# print(f'min_i -> {min}')

	# times = {i:0 for i in range(int(min), int(max)+5, 5)}
	# ticks = [i-2.5 for i in range(int(min), int(max)+10, 5)]
	# alt_ticks = []
	# for tick in ticks:
	# 	alt_ticks.append(tick)
	# 	if tick == -2.5:
	# 		alt_ticks.append(0)
	# ticks = alt_ticks
	
	# for obj in json:
	# 	t = (obj['original_travel_time'] - obj['generate_travel_time'])/60
	# 	i = 0
	# 	if t < 0:
	# 		i = (math.ceil(t / 2.5) * 2.5)
	# 		if i%5!=0:
	# 			i -= 2.5
	# 	elif t > 0:
	# 		i = (math.floor(t / 2.5) * 2.5)
	# 		if i%5!=0:
	# 			i += 2.5
	# 	else:
	# 		i = 0

	# 	times[i] += obj['passengers']

	# del times[0]

	# colors = []
	# for key in times:
	# 	if key == 0:
	# 		colors.append('gray')
	# 	elif key < 0:
	# 		colors.append('green')
	# 	else:
	# 		colors.append('red')

	bin_edges_right = [2.5]
	while bin_edges_right[-1] < max_t:
		bin_edges_right.append(bin_edges_right[-1]+5)
	
	bin_edges_left = [-2.5]
	while bin_edges_left[0] > min_t:
		bin_edges_left = [bin_edges_left[0]-5] + bin_edges_left

	bin_edges = bin_edges_left + bin_edges_right

	print(bin_edges)

	plt.figure(figsize=(16, 6))
	# plt.bar(times.keys(), times.values(), color=colors, width=5, align='center', edgecolor='black')
	plt.hist(x_pos, weights=w_pos, bins=bin_edges, color='red', edgecolor='darkred')
	plt.hist(x_neg, weights=w_neg, bins=bin_edges, color='green', edgecolor='darkgreen')
	plt.hist(x_zero, weights=w_zero, bins=bin_edges, color='grey', edgecolor='black')
	# plt.yscale('log', base=10)
	plt.ylabel('Passenger Count')
	plt.xlabel('Travel time difference from the original network (mins)')
	plt.xticks(bin_edges)
	plt.grid(axis='y')
	plt.savefig(f'../data/images/network-{INDEX}-time.png', bbox_inches='tight', dpi=400)
	if SHOW:
		plt.show()


if __name__ == '__main__':
	INDEX = int(sys.argv[1])
	SHOW  = False 

	json = utils.json_utils.read_json_object(f'../data/json/comparisons/comparison-{INDEX}.json')
	plt.rcParams['font.size'] = '12'

	tots = 0
	for obj in json:
		tots += obj['passengers']
	print(f'Total passengers: {tots}')

	do_transfers(json)
	do_travel_time(json)