
import matplotlib.pyplot as plt

if __name__ == '__main__':
	x = ['lisbon original', 'lisbon improve', 'our best']
	colors = ['gray', 'crimson', 'darkcyan']
	y_average_wait = [4.022, 3.706, 3.39]
	y_distance = [1.13925e+06, 508051, 457464]
	
	fig, axs = plt.subplots(1, 2)
	axs[0].bar(x, y_average_wait, color=colors)
	axs[0].grid(axis='y')
	axs[0].set_ylabel('Average waiting time (min)')
	axs[1].bar(x, y_distance, color=colors)
	axs[1].grid(axis='y')
	axs[1].set_ylabel('Total distance traveled (Km)')
	plt.show()