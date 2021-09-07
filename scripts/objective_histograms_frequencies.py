import utils.json_utils
import matplotlib.pyplot as plt
import sys

indexes   = [0, 24, 49, 74, 99, 124, 149, 174, 199]
lisbon_costs = [
	0.0670429,
	1.13925e+06
]

labels = [
	"Average waiting time (mins)",
	"Total distance travelled (Km)",
]

if __name__ == '__main__':
	highlight = None

	try:
		highlight = int(sys.argv[1])
	except ValueError:
		highlight = 'naught'

	json = utils.json_utils.read_json_object('../data/json/run_frequencies.json')

	indexes = [index for index in indexes if index <= len(json)-1]

	for index in indexes:
		print(f'n{index} -> {json[index]["costs"][0]} | {json[index]["costs"][1]}')

	print(f'lisbon -> {lisbon_costs[0]} | {lisbon_costs[1]}')
		
	total_km   = 0
	total_wait = 1

	fig, axs = plt.subplots(1, 2)

	for i in range(2):
		x = ["lisbon"]
		[x.append(f'n{index}') for index in indexes]
		y = [lisbon_costs[i]]
		[y.append(json[index]['costs'][i]) for index in indexes]
		colors = ['gray']
		[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
		if i==0:
			y = [n*60 for n in y]
		axs[i].bar(x, y, color=colors)
		axs[i].set_ylabel(labels[i])
		axs[i].grid(axis='y')
		# axs[i].bar(x, y, color=colors)
		# axs[i].set_ylabel(labels[i])
		# axs[i].grid(axis='y')

	# plt.savefig(f'../data/images/network-{highlight}-highlight.png', bbox_inches='tight', dpi=400)
	plt.show()
	plt.clf()

	# costs = [json[i]['costs'] for i in indexes]
	# costs = [lisbon_costs, net_300_costs, net_norm_costs] + costs
	# costs = [compute_cost_function(c) for c in costs]
	# labels = ['lisbon', 'weighted', 'uniform'] + [f'n{n}' for n in indexes]
	# colors = ['gray', 'crimson', 'darkcyan'] + ['cornflowerblue' for _ in indexes]
	# plt.bar(labels, costs, color=colors)
	# plt.ylabel('Weighted Objective Function Value')
	# plt.grid(axis='y')
	# plt.show()