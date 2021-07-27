import utils.json_utils
import matplotlib.pyplot as plt

indexes   = [0, 24, 49, 74, 99, 124, 149, 174, 199]
highlight = 0 #'naught'
lisbon_costs = [
	3.56122e+06, # total length (in meters)
	5.77707e-06, # unsatisfied demand 
	1.39027e+07, # in vehicle time (in seconds)
	0.0305169    # average transfers ()
]

labels = [
	"Total Length (meters)",
	"Unsatisfied Demand",
	"In Vehicle Time (seconds)",
	"Average Trasnfers"
]

if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/run_nets.json')

	for index in indexes:
		print(f'n{index} -> {len(json[index]["routes"])}')

	total_length = 0
	unsatisfied  = 1
	in_vehicle   = 2
	transfers    = 3

	fig, axs = plt.subplots(2, 2, figsize=(13, 10))
	# fig, axs = plt.subplots(1, 4, figsize=(24, 5))

	for i in range(4):
		x = ["lisbon"]
		[x.append(f'n{index}') for index in indexes]
		y = [lisbon_costs[i]]
		[y.append(json[index]['costs'][i]) for index in indexes]
		colors = ['gray']
		[colors.append('orange' if index==highlight else 'cornflowerblue') for index in indexes]
		axs[i//2, i%2].bar(x, y, color=colors)
		axs[i//2, i%2].set_ylabel(labels[i])
		axs[i//2, i%2].grid(axis='y')

	plt.savefig(f'../data/images/network-{highlight}-highlight.png', bbox_inches='tight', dpi=400)