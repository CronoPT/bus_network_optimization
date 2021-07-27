import utils.json_utils
import matplotlib.pyplot as plt

OBJECTIVES = 4
LABELS = ['Total Length', 'Unsatisfied Demand', 'In Vehicle Time', 'Average Transfers']

lisbon_costs = [
	3.56122e+06, # total length (in meters)
	5.77707e-06, # unsatisfied demand 
	1.39027e+07, # in vehicle time (in seconds)
	0.0305169    # average transfers ()
]

if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/runs/tnfsp.json')
	json = json['summary'][-1]

	fig, axs = plt.subplots(OBJECTIVES, OBJECTIVES, figsize=(10, 10), sharex='col', sharey='row')
	
	# pad = 5
	# for ax, col in zip(axs[0], LABELS):
	# 	ax.annotate(col, xy=(0.5, 1), xytext=(0, pad),
	# 				xycoords='axes fraction', textcoords='offset points',
	# 				size='large', ha='center', va='baseline')
	# for ax, row in zip(axs[:,0], LABELS):
	# 	ax.annotate(row, xy=(0, 0.5), xytext=(-ax.yaxis.labelpad - pad, 0),
	# 				xycoords=ax.yaxis.label, textcoords='offset points',
	# 				size='large', ha='right', va='center')


	for i in range(OBJECTIVES):
		y = [n[i] for n in json]
		for j in range(OBJECTIVES):
			if i > j:
				x = [n[j] for n in json]
				axs[i, j].scatter(x, y)
				axs[i, j].scatter([lisbon_costs[j]], [lisbon_costs[i]])
				continue
			elif i == j:
				axs[i, j].annotate(LABELS[i], (0.5, 0.5), xycoords='axes fraction',
                    ha='center', va='center', size='large')
			
			axs[i, j].tick_params(
				axis='both',
				which='both',
				bottom=False,
				top=False, 
				left=False,
				right=False,
				labelbottom=False,
				labelleft=False
			)
			

	
	axs[OBJECTIVES-1, OBJECTIVES-1].set_xticks([])

	plt.show() 