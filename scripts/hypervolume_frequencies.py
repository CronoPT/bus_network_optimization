import utils.json_utils
from pymoo.factory import get_performance_indicator
import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join

def plot_individual(filename):
	run_summary = utils.json_utils.read_json_object(filename)

	hypervolumes = []
	hv = get_performance_indicator("hv", ref_point=np.array([0.5, 1000000]))
	for iteration in run_summary['summary']:
		hypervolumes.append(hv.calc(np.array(iteration)))

	plt.plot(
		range(len(hypervolumes)), 
		hypervolumes
	)
	plt.grid(b=True)
	plt.legend()
	plt.ylabel('Hypervolume')
	plt.xlabel('Iterations')
	plt.show()

if __name__ == '__main__':
	plot_individual('../data/json/runs/tnfsp_frequencies.json')
