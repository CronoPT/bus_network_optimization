import utils.json_utils
import matplotlib.pyplot as plt
from matplotlib.patches import Patch
import sys

def compute_cost_function(costs):
	weights = [
		-15.851532967952707, 
		1.0391450846092443e-06, 
		53.132458885735744, 
		7.747345808405972e-09, 
		72.38214508631111
	]

	cost = weights[0]
	for i in range(len(costs)):
		cost += weights[i+1]*costs[i]
	return cost

indexes   = [0, 24, 49, 74, 99, 124, 149, 174, 199]

lisbon_costs = [
	3.56122e+06, # total length (in meters)
	0.00698877, # unsatisfied demand 
	2.49523e+08, # in vehicle time (in seconds)
	0.187626, # average transfers ()
]

net_300_costs = [
	2.33477e+06,
	0.013274,
	2.44807e+08,
	0.177974
]

net_norm_costs = [
	2.20581e+06,
	0.0184234,
	2.44167e+08,
	0.183274
]

labels = [
	"Total Length (meters)",
	"Unsatisfied Demand",
	"In Vehicle Time (seconds)",
	"Average Transfers"
]

if __name__ == '__main__':

	plt.rcParams.update({'font.size': 14})

	highlight = None

	try:
		highlight = int(sys.argv[1])
	except ValueError:
		highlight = 'naught'

	json = utils.json_utils.read_json_object('../data/json/run_nets.json')

	print(f'Original Net OF (  )-> {compute_cost_function(lisbon_costs)}')
	print(f'Improved Net OF (SO)-> {compute_cost_function(net_300_costs)}')
	print(f'Improved Net OF (MO)-> {compute_cost_function(json[0]["costs"])}')

	for index in indexes:
		print(f'n{index} -> {len(json[index]["routes"])} | {compute_cost_function(json[index]["costs"])}')

	json_weighted_sum = utils.json_utils.read_json_object('../data/json/run_nets_single_300.json')
	print(f'weighted_sum -> {len(json_weighted_sum[0]["routes"])} | {compute_cost_function(json_weighted_sum[0]["costs"])}')
	json_uniform_sum  = utils.json_utils.read_json_object('../data/json/run_nets_single.json')
	print(f'uniform_sum -> {len(json_uniform_sum[0]["routes"])} | {compute_cost_function(json_uniform_sum[0]["costs"])}')

	total_length = 0
	unsatisfied  = 1
	in_vehicle   = 2
	transfers    = 3

	# fig, axs = plt.subplots(4, 1, figsize=(9, 24))
	fig, axs = plt.subplots(2, 2, figsize=(25, 15))
	# fig, axs = plt.subplots(1, 4, figsize=(24, 5))

	# for i in range(4):
	# 	x = ["lisbon"]
	# 	[x.append(f'n{index}') for index in indexes]
	# 	y = [lisbon_costs[i]]
	# 	[y.append(json[index]['costs'][i]) for index in indexes]
	# 	colors = ['gray']
	# 	[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
	# 	axs[i//2, i%2].bar(x, y, color=colors)
	# 	axs[i//2, i%2].set_title(labels[i])
	# 	axs[i//2, i%2].grid(axis='y')
	# 	# axs[i].bar(x, y, color=colors)
	# 	# axs[i].set_ylabel(labels[i])
	# 	# axs[i].grid(axis='y')

	legend_elements = [
		Patch(
			facecolor='grey', edgecolor='grey',
			label='Current Network'
		),
		Patch(
			facecolor='crimson', edgecolor='crimson',
			label='SO with Weighted Function'
		),
		Patch(
			facecolor='darkcyan', edgecolor='darkcyan',
			label='SO with Uniform Function'
		),
		Patch(
			facecolor='cornflowerblue', edgecolor='cornflowerblue',
			label='Pareto Front Networks'
		),
	]

	for i in range(4):
		x = ["lisbon", "wei", "uni"]
		[x.append(f'n{index}') for index in indexes]
		y = [lisbon_costs[i], net_300_costs[i], net_norm_costs[i]]
		[y.append(json[index]['costs'][i]) for index in indexes]
		colors = ['gray', 'crimson', 'darkcyan']
		[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
		axs[i//2, i%2].bar(x, y, color=colors)
		axs[i//2, i%2].set_title(labels[i])
		axs[i//2, i%2].grid(axis='y')
		# axs[i].bar(x, y, color=colors)
		# axs[i].set_ylabel(labels[i])
		# axs[i].grid(axis='y')

	# axs[0].legend(handles=legend_elements, loc='upper left', ncol=2, bbox_to_anchor=(0, 1.3))

	# axs[0][0].legend(handles=legend_elements, loc='upper left', ncol=2, bbox_to_anchor=(0.7, 1.3))

	# for i in range(4):
	# 	x = ["lisbon", "wei", "uni"]
	# 	[x.append(f'n{index}') for index in indexes]
	# 	y = [lisbon_costs[i], net_300_costs[i], net_norm_costs[i]]
	# 	[y.append(json[index]['costs'][i]) for index in indexes]
	# 	colors = ['gray', 'crimson', 'darkcyan']
	# 	[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
	# 	axs[i].bar(x, y, color=colors)
	# 	axs[i].set_title(labels[i])
	# 	axs[i].grid(axis='y')
	# 	# axs[i].bar(x, y, color=colors)
	# 	# axs[i].set_ylabel(labels[i])
	# 	# axs[i].grid(axis='y')

	# plt.savefig(f'../data/images/network-{highlight}-highlight.png', bbox_inches='tight', dpi=400)
	# plt.savefig(f'../data/images/network-{highlight}-highlight-for-summary.png', bbox_inches='tight', dpi=400)
	plt.savefig(f'../data/images/network-{highlight}-highlight-single.png', bbox_inches='tight', dpi=400)

	plt.clf()

	plt.rcParams.update({'font.size': 14})
	plt.figure(figsize=(12, 6))
	costs = [json[i]['costs'] for i in indexes]
	costs = [lisbon_costs, net_300_costs, net_norm_costs] + costs
	costs = [compute_cost_function(c) for c in costs]
	labels = ['lisbon', 'wei', 'uni'] + [f'n{n}' for n in indexes]
	colors = ['gray', 'crimson', 'darkcyan'] + ['cornflowerblue' for _ in indexes]
	plt.bar(labels, costs, color=colors)
	plt.title('Weighted Objective Function Value')
	plt.grid(axis='y')
	plt.savefig(f'../data/images/objective_function_all.png', bbox_inches='tight', dpi=400)
	# plt.show()