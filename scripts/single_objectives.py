import utils.json_utils
import matplotlib.pyplot as plt

if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/runs/tndp_single.json')
	weights = [
		-15.851532967952707, 
		1.0391450846092443e-06, 
		53.132458885735744, 
		7.747345808405972e-09, 
		72.38214508631111
	]

	x = []
	for iteration in json['summary']:
		total = 0
		for individual in iteration:
			total += weights[0]
			for i, cost in enumerate(individual):
				total += cost*weights[i+1] 
			total /= len(individual)
		x.append(total)

	plt.plot(x)
	plt.grid(axis='y')
	plt.show()