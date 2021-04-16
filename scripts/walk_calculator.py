
'''
| This script implements the projStops step in Vuurstaek et al.
| It does so by first indexing every road segment to a square
| by dividing the space spanned by the network into a grid.
| This indexing allows us to compute projections to a small
| amount of road segments instead of them all which takes hours
| as oposed to this script which is much faster.
'''


import pandas as pd
import numpy as np
import math
import networkx as nx
import utils.general_utils
import utils.geometric_utils
import utils.json_utils
import configs


def coord_to_square(lon, lat):
	'''
	| This function maps a coordinate pair to 
	| a x&y position in the greed.
	| x indexes longitude and y indexes latitude.
	'''
	x = math.floor( (lon-min_lon) / lon_step )
	y = math.floor( (lat-min_lat) / lat_step )
	x = x if x < configs.DIVISIONS else x-1
	y = y if y < configs.DIVISIONS else y-1
	return x, y


def int_metro_id(metro_id):
	return int(metro_id.split('M')[1])


def find_stop_item(id):
	global bus_stops
	return list(filter(lambda item: item['stop_id']==id, bus_stops))[0]


def compute_candidates(i, j):
	candidate_coords = [[i, j]]
	
	if i > 0:
		candidate_coords.append([i-1, j])

	if j > 0:
		candidate_coords.append([i, j-1])

	if i < configs.DIVISIONS-1:
		candidate_coords.append([i+1, j])
	
	if j < configs.DIVISIONS-1:
		candidate_coords.append([i, j+1])

	if i>0 and j>0:
		candidate_coords.append([i-1, j-1])

	if i<configs.DIVISIONS-1 and j<configs.DIVISIONS-1:
		candidate_coords.append([i+1, j+1])

	if i>0 and j<configs.DIVISIONS-1:
		candidate_coords.append([i-1, j+1])

	if i<configs.DIVISIONS-1 and j>0:
		candidate_coords.append([i+1, j-1])

	return candidate_coords

if __name__ == '__main__':

	metro_stations = utils.json_utils.read_json_object(configs.METRO_STATIONS)
	bus_stops = utils.json_utils.read_json_object(configs.BUS_STOPS)

	max_lat = -np.inf
	min_lat = np.inf
	max_lon = -np.inf
	min_lon = np.inf

	# Just assessing the limits of the grid
	for index, bus_stop in enumerate(bus_stops):
		utils.general_utils.print_progress_bar(index+1, len(bus_stops), prefix='[BUS LIMITS]  ')

		lon = bus_stop['point'][0]
		lat = bus_stop['point'][1]

		max_lat = lat if lat > max_lat else max_lat
		min_lat = lat if lat < min_lat else min_lat
		max_lon = lon if lon > max_lon else max_lon
		min_lon = lon if lon < min_lon else min_lon

	for index, (metro_id, metro_station) in enumerate(metro_stations.items()):
		utils.general_utils.print_progress_bar(index+1, len(metro_stations), prefix='[METRO LIMITS]')
		lon = metro_station['point'][0]
		lat = metro_station['point'][1]

		max_lat = lat if lat > max_lat else max_lat
		min_lat = lat if lat < min_lat else min_lat
		max_lon = lon if lon > max_lon else max_lon
		min_lon = lon if lon < min_lon else min_lon	
		
	bounds = [[{} for i in range(configs.DIVISIONS)] for i in range(configs.DIVISIONS)]
	lat_step = (max_lat-min_lat)/configs.DIVISIONS
	lon_step = (max_lon-min_lon)/configs.DIVISIONS

	for index_ou, x in enumerate(range(configs.DIVISIONS)):
		for index_in, y in enumerate(range(configs.DIVISIONS)):
			utils.general_utils.print_progress_bar(
				(index_in+1) + configs.DIVISIONS*index_ou, 
				configs.DIVISIONS**2,
				prefix='[FILL SQUARES]')

			bounds[x][y] = {
				'max_lat': min_lat + (y+1)*lat_step,
				'min_lat': min_lat + y*lat_step,
				'max_lon': min_lon + (x+1)*lon_step,
				'min_lon': min_lon + x*lon_step,
				'metro_stations': [],
				'bus_stops': []
			}

	for index, bus_stop in enumerate(bus_stops):
		utils.general_utils.print_progress_bar(index+1, len(bus_stops), prefix='[BUS FILL]    ')
		lon = bus_stop['point'][0]
		lat = bus_stop['point'][1]
		i,j = coord_to_square(lon, lat)
		bounds[i][j]['bus_stops'].append(bus_stop['stop_id'])

	for index, (metro_id, metro_station) in enumerate(metro_stations.items()):
		utils.general_utils.print_progress_bar(index+1, len(metro_stations), prefix='[METRO FILL]  ')
		lon = metro_station['point'][0]
		lat = metro_station['point'][1]
		i,j = coord_to_square(lon, lat)
		bounds[i][j]['metro_stations'].append(int_metro_id(metro_id))

	important_points = {}

	for index, bus_stop in enumerate(bus_stops):
		utils.general_utils.print_progress_bar(index+1, len(bus_stops), prefix='[BUS CHECK]   ')

		important_points[bus_stop['stop_id']] = {
			'type': 'bus',
			'close_bus': [],
			'close_metro': []
		}

		lon = bus_stop['point'][0]
		lat = bus_stop['point'][1]
		i,j = coord_to_square(lon, lat)

		candidate_coords = compute_candidates(i, j)
		for i_prime, j_prime in candidate_coords:
			for bus_prime in bounds[i_prime][j_prime]['bus_stops']:
				if bus_prime == bus_stop['stop_id']:
					continue
				point = [lon, lat]
				point_prime = find_stop_item(bus_prime)['point']
				distance = utils.geometric_utils.haversine_distance(
					point,
					point_prime
				)
				if distance < configs.MAX_DIST_METRO_BUS:
					important_points[bus_stop['stop_id']]['close_bus'].append({
						'stop_id': bus_prime,
						'time': (distance / (configs.WALKING_SPEED / 3.6))
					})
			for metro_prime in bounds[i_prime][j_prime]['metro_stations']:
				point = [lon, lat]
				point_prime = metro_stations[f'M{metro_prime:02}']['point']
				distance = utils.geometric_utils.haversine_distance(
					point,
					point_prime
				)
				if distance < configs.MAX_DIST_METRO_BUS:
					important_points[bus_stop['stop_id']]['close_bus'].append({
						'station_id': bus_prime,
						'time': (distance / (configs.WALKING_SPEED / 3.6))
					})


	for index, (metro_id, metro_station) in enumerate(metro_stations.items()):
		utils.general_utils.print_progress_bar(index+1, len(metro_stations), prefix='[METRO CHECK] ')
		id = int_metro_id(metro_id)
		important_points[id] = {
			'type': 'metro',
			'close_bus': [],
			'close_metro': []
		}

		lon = metro_station['point'][0]
		lat = metro_station['point'][1]
		i,j = coord_to_square(lon, lat)

		candidate_coords = compute_candidates(i, j)
		for i_prime, j_prime in candidate_coords:
			for bus_prime in bounds[i_prime][j_prime]['bus_stops']:
				point = [lon, lat]
				point_prime = find_stop_item(bus_prime)['point']
				distance = utils.geometric_utils.haversine_distance(
					point,
					point_prime
				)
				if distance < configs.MAX_DIST_METRO_BUS:
					important_points[id]['close_bus'].append({
						'stop_id': bus_prime,
						'time': (distance / (configs.WALKING_SPEED / 3.6))
					})

	utils.json_utils.write_json_object(configs.WALKS, important_points)
