import utils.json_utils
from pymoo.factory import get_performance_indicator
import numpy as np
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join

def line_name(summary):
	mut  = summary['mutation_probability']
	ite  = summary['iterations']
	pop  = summary['population_size']
	name = f'mutation: {mut}, pop: {pop}'
	return name

def plot_set(ax, file_set, title):
	for file in file_set:
		if f'{mypath}{file}' in PURGE:
			continue
		run_summary = utils.json_utils.read_json_object(f'{mypath}{file}')

		hypervolumes = []
		hv = get_performance_indicator("hv", ref_point=np.array([8e+06, 1, 5e+07, 1]))
		for iteration in run_summary['summary']:
			hypervolumes.append(hv.calc(np.array(iteration)))

		hypervolumes = hypervolumes[:120]

		ax.plot(
			range(len(hypervolumes)), 
			hypervolumes, 
			label=line_name(run_summary)
		)
		ax.grid(b=True)
		ax.legend()
		ax.set_ylabel('Hypervolume')
		ax.set_xlabel('Iterations')
		ax.set_title(title)


def plot_individual(filename):
	run_summary = utils.json_utils.read_json_object(filename)

	hypervolumes = []
	hv = get_performance_indicator("hv", ref_point=np.array([8e+06, 1, 5e+08, 1]))
	for iteration in run_summary['summary']:
		hypervolumes.append(hv.calc(np.array(iteration)))

	plt.plot(
		range(len(hypervolumes)), 
		hypervolumes, 
		label=line_name(run_summary)
	)
	plt.grid(b=True)
	plt.legend()
	plt.ylabel('Hypervolume')
	plt.xlabel('Iterations')
	plt.show()

if __name__ == '__main__':
	INDIVIDUAL = False
	PURGE = [
		'../data/json/runs/tnfsp.json',
		'../data/json/runs/tnfsp_80.json',
		'../data/json/runs/tnfsp_1.json'
	]

	if INDIVIDUAL:
		plot_individual('../data/json/runs/tnfsp.json')
	else:
		mypath = '../data/json/runs/'
		files  = [f for f in listdir(mypath) if isfile(join(mypath, f))]

		population_size = {}
		mutation_prob   = {}

		sizes = [50]
		probs = [0.1]

		for file in files:
			run_summary = utils.json_utils.read_json_object(f'{mypath}{file}')

			if run_summary['population_size'] in population_size:
				population_size[run_summary['population_size']].append(file)
			else:
				population_size[run_summary['population_size']] = [file]

			if run_summary['mutation_probability'] in mutation_prob:
				mutation_prob[run_summary['mutation_probability']].append(file)
			else:
				mutation_prob[run_summary['mutation_probability']] = [file]

		fig, axs = plt.subplots(1, 2, sharey=True, figsize=(12, 5))
		plot_set(axs[0], population_size[50], 'Varying Mutation Prob')
		plot_set(axs[1], mutation_prob[0.1], 'Varying Pop Size')
		
		plt.show()