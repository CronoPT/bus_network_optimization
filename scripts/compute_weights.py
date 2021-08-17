import utils.json_utils
import numpy as np
import scipy.optimize.nnls
import scipy.optimize._lsq.lsq_linear

def least_squares_standard(X, t):
	return np.dot(np.dot(np.linalg.inv(np.dot(np.matrix.transpose(X), X)), np.matrix.transpose(X)), t)

def least_squares_non_negative(X, t):
	return scipy.optimize.nnls(X, t)

def least_squares_all_positive(X, t):
	return scipy.optimize._lsq.lsq_linear(X, t, bounds=(0, np.inf))

if __name__ == '__main__':
	indexes = [0, 24, 49, 74, 99, 124, 149, 174, 199]
	json = utils.json_utils.read_json_object('../data/json/run_nets.json')

	X = []
	for i in indexes:
		X.append([1] + json[i]['costs'][0:4])

	X = np.array(X)

	t = [
		7.5, 
		3.5, 
		5, 
		5.75,
		3.25, 
		6, 
		7.5, 
		5.5, 
		4.5
	]

	t = np.array([(10-n) for n in t])
	
	print(X)
	print(t)

	w1 = least_squares_standard(X, t)
	# w2 = least_squares_non_negative(X, t)[0]
	# w3 = least_squares_all_positive(X, t)['x']

	x1 = np.dot(X, w1)
	# x2 = np.dot(X, w2)
	# x3 = np.dot(X, w3)

	print(f'w1 -> {[i for i in w1]}')
	# # print(f'x1 -> {np.dot(X, w1)}')
	# print(f'w2 -> {[i for i in w2]}')
	# # print(f'x2 -> {np.dot(X, w2)}')
	# print(f'w3 -> {[i for i in w3]}')
	# # print(f'x3 -> {np.dot(X, w3)}')

	# for i in range(X.shape[0]):
	# 	print(f'rating-> {t[i]}| x1 -> {x1[i]} | x2 -> {x2[i]} | x3 -> {x3[i]}')