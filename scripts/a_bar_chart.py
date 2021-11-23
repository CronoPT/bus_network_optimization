
import matplotlib.pyplot as plt

if __name__ == '__main__':
	x = ['lisbon original', 'lisbon improve', 'our best']
	colors = ['gray', 'crimson', 'darkcyan']
	y_average_wait = [4.022, 3.706, 3.39]
	y_distance = [1.13925e+06, 508051, 457464]
	
	plt.rcParams.update({'font.size': 12})
	fig, axs = plt.subplots(1, 2, figsize=(12, 5))
	axs[0].bar(x, y_average_wait, color=colors)
	axs[0].grid(axis='y')
	axs[0].set_title('Average waiting time (min)')
	axs[1].bar(x, y_distance, color=colors)
	axs[1].grid(axis='y')
	axs[1].set_title('Total distance traveled (Km)')
	plt.savefig(f'../data/images/a_bar_chart.png', bbox_inches='tight', dpi=400)