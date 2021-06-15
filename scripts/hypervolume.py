import utils.json_utils
from pymoo.factory import get_performance_indicator
import numpy as np
import matplotlib.pyplot as plt

if __name__ == '__main__':
	run_summary = utils.json_utils.read_json_object('../data/json/runs/tnfsp.json')

	hypervolumes = []
	hv = get_performance_indicator("hv", ref_point=np.array([8e+06, 1, 5e+07, 1]))
	for iteration in run_summary['summary']:
		hypervolumes.append(hv.calc(np.array(iteration)))

	plt.scatter(range(len(run_summary['summary'])), hypervolumes)
	plt.title('Hypervolume over the GA iterations')
	plt.ylabel('Hypervolume')
	plt.xlabel('GA Iterations')
	plt.show()