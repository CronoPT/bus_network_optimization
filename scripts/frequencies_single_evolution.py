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
		'file': '../data/json/run_frequencies_single_total.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_single_total.json',
		'label': 'Full Day',
		'hours': 24,
		'bottom': 0,
		'wait_max': 0.128749,
		'wait_min': 0.0532552,
		'distance_max': 980918,
		'distance_min': 501343
	},
	'morning': {
		'file': '../data/json/run_frequencies_single_morning.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_single_morning.json',
		'label': 'Morning Rush Hour',
		'hours': 3,
		'bottom': 0,
		'color': 'khaki',
		'wait_max': 0.129927,
		'wait_min': 0.0551362,
		'distance_max': 123862,
		'distance_min': 63675.8
	},
	'midday': {
		'file': '../data/json/run_frequencies_single_midday.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_single_midday.json',
		'label': 'Middle of Day',
		'hours': 6,
		'bottom': 0,
		'color': 'mediumseagreen',
		'wait_max': 0.135387,
		'wait_min': 0.0515793,
		'distance_max': 243893,
		'distance_min': 129018
	},
	'afternoon': {
		'file': '../data/json/run_frequencies_single_afternoon.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_single_afternoon.json',
		'label': 'Afternoon Rush Hour',
		'hours': 3,
		'bottom': 0,
		'color': 'lightcoral',
		'wait_max': 0.13001,
		'wait_min': 0.0524471,
		'distance_max': 122494,
		'distance_min': 83570.2
	},
	'happy': {
		'file': '../data/json/run_frequencies_single_happy.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_single_happy.json',
		'label': 'Happy Hour',
		'hours': 4,
		'bottom': 0,
		'color': 'mediumpurple',
		'wait_max': 0.119427,
		'wait_min': 0.0477746,
		'distance_max': 164708,
		'distance_min': 82332.5
	},
	'night': {
		'file': '../data/json/run_frequencies_single_night.json',
		'summary_file': '../data/json/runs/tnfsp_frequencies_single_night.json',
		'label': 'Night Time',
		'hours': 8,
		'bottom': 0,
		'color': 'skyblue',
		'wait_max': 0.120607,
		'wait_min': 0.0495152,
		'distance_max': 326317,
		'distance_min': 160220
	}
}

if __name__ == '__main__':

	for time, item in times.items():
		item['json_summary'] = utils.json_utils.read_json_object(item['summary_file'])

	fig, axs = plt.subplots(1, 2)

	y = [[] for time in times if time != 'total']
	labels = [time for time in times if time != 'total']
	colors = [times[label]['color'] for label in labels] 
	for t, time in enumerate(labels):
		for iteration in times[time]['json_summary']['summary']:
			total = 0
			for costs in iteration:
				total += costs[1]
			total /= len(iteration)
			y[t].append(total)
	axs[1].stackplot(range(301), y, labels=labels, colors=colors)
	
	
	y_total = []
	for iteration in times['total']['json_summary']['summary']:
		total = 0
		for costs in iteration:
			total += costs[1]
		total /= len(iteration)
		y_total.append(total)
	axs[1].plot(y_total, color='black', label='total')

	y_total = []
	for iteration in times['total']['json_summary']['summary']:
		total = 0
		for costs in iteration:
			total += costs[0]
		total /= len(iteration)
		y_total.append(total)
	y_total = [n*60 for n in y_total]
	axs[0].plot(y_total, color='black', label='total')

	y_separate = [0 for i in range(301)]
	for t, time in enumerate(labels):
		for index, iteration in enumerate(times[time]['json_summary']['summary']):
			total = 0
			for costs in iteration:
				total += costs[0]
			total /= len(iteration)
			y_separate[index] += total*(times[time]['hours']/24)
	y_separate = [n*60 for n in y_separate]
	axs[0].plot(y_separate, color='red', label='separate')


	plt.legend()
	plt.grid(axis='y')
	plt.show()

	plt.clf()

	y_total = []
	for iteration in times['total']['json_summary']['summary']:
		total = 0
		for costs in iteration:
			total += (costs[0]-times['total']['wait_min']) / (times['total']['wait_max']-times['total']['wait_min']) 
			total += (costs[1]-times['total']['distance_min']) / (times['total']['distance_max']-times['total']['distance_min'])
		total /= len(iteration)
		y_total.append(total)
	plt.plot(y_total, color='black', label='total')

	y_separate = [0 for i in range(301)]
	for t, time in enumerate(labels):
		for index, iteration in enumerate(times[time]['json_summary']['summary']):
			total = 0
			for costs in iteration:
				total += (costs[0]-times[time]['wait_min']) / (times[time]['wait_max']-times[time]['wait_min']) 
				total += (costs[1]-times[time]['distance_min']) / (times[time]['distance_max']-times[time]['distance_min'])
			total /= len(iteration)
			y_separate[index] += total*(times[time]['hours']/24)
	plt.plot(y_separate, color='red', label='separate')
	plt.legend()
	plt.grid(axis='y')
	plt.show()


				
	
