
import psycopg2
import matplotlib.pyplot as plt
import geojsonio
import geopandas
import json
import pandas as pd
import numpy as np
from shapely.geometry import shape
from scipy.stats import multivariate_normal
import seaborn as sns 
import utils.json_utils 

LISBON = True
INDEX  = 0

def plot_routes_per_stop(net):
	stops = {}

	for route in net:
		for stop in route:
			if stop not in stops:
				stops[stop] = 0
			stops[stop] += 1

	series = [count for _, count in stops.items()]
	positions = np.arange(min(series), max(series)*(13/12), max(series)//12, dtype=int)
	sns.displot(series, bins=positions)
	plt.xticks(positions)
	# plt.title('Number of routes serving each stop in the current Network')
	plt.xlabel('Number of routes serving each stop')
	plt.ylabel('Stop count')
	plt.show()

def plot_stops_per_route(net):
	series = [len(route) for route in net]
	positions = np.arange(min(series), max(series)*(13/12), max(series)//12, dtype=int)
	sns.displot(series, bins=positions)
	plt.xticks(positions)
	# plt.title('Number of stops per route in the current Network')
	plt.xlabel('Number of stops per route')
	plt.ylabel('Route count')
	plt.show()

if __name__ == '__main__':
	json = utils.json_utils.read_json_object('../data/json/run_nets.json')
	lisbon_json = utils.json_utils.read_json_object('../data/json/clustered_routes_stop_sequence.json')
	lisbon_json = [obj['stops'] for obj in lisbon_json]

	net = None
	if LISBON:
		net = lisbon_json
	else:
		net = json[INDEX]['routes']

	plot_routes_per_stop(net)
	plot_stops_per_route(net)
