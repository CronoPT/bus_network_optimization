import utils.json_utils
import matplotlib.pyplot as plt
from os import listdir
from os.path import isfile, join

def plot_collective():
	mypath = '../data/json/runs/'
	files  = [f for f in listdir(mypath) if isfile(join(mypath, f))]

	no_nos = ['tndp_single.json', 'tndp_single_300.json']
	labels = {'tndp_single.json':'Uniform SO', 'tndp_single_300.json':'Weighted SO'}
	# no_nos = ['tndp_single_300.json']

	for file in files:
		if 'single' in file and file in no_nos:
			json = utils.json_utils.read_json_object(f'{mypath}{file}')

			x = []
			for iteration in json['summary']:
				total = 0
				for individual in iteration:
					total += weights[0]
					for i, cost in enumerate(individual):
						total += cost*weights[i+1] 
				total /= len(iteration)
				x.append(total)

			plt.plot(
				x, 
				label=labels[file] #f'crossover {json["crossover_probability"]}'
			)

	original_values = [
		2.872985364118888,
		6.79936712824335,
		4.840498525797122,
		3.8583516765914645,
		6.745754171817664,
		3.3225436637882755,
		3.2629063327186696,
		4.85235592735024,
		4.9452372088467484
	]
		
	plt.hlines(original_values, 0, 300, colors='thistle', label='MO Function Values')
	plt.grid(b=True)
	plt.legend()
	plt.ylabel('Objetive Function Value')
	plt.xlabel('Iterations')
	# plt.set_title(title)
	plt.show()

def plot_single():
	json = utils.json_utils.read_json_object('../data/json/runs/tndp_single.json')
	x = []
	for iteration in json['summary']:
		total = 0
		for individual in iteration:
			total += weights[0]
			for i, cost in enumerate(individual):
				total += cost*weights[i+1] 
		total /= len(iteration)
		x.append(total)

	plt.plot(x)
	plt.grid(axis='y')
	plt.show()

if __name__ == '__main__':
	weights = [
		-15.851532967952707, 
		1.0391450846092443e-06, 
		53.132458885735744, 
		7.747345808405972e-09, 
		72.38214508631111
	]

	SINGLE = False

	if SINGLE:
		plot_single()
	else:
		plot_collective()