import psycopg2
import configs
import utils.json_utils
import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import datetime
import time
import itertools

user = 'postgres'
password = 'rootroot'
host = '127.0.0.1'
port = '5432'
database = 'iludb'

def coord_to_square(lon, lat):
	'''
	| This function maps a coordinate pair to 
	| a x&y position in the greed.
	| x indexes longitude and y indexes latitude.
	'''

	global lon_step
	global lat_step
	global _DIVISIONS

	x = math.floor( (lon-min_lon) / lon_step )
	y = math.floor( (lat-min_lat) / lat_step )
	x = x if x < _DIVISIONS else x-1
	y = y if y < _DIVISIONS else y-1
	return x, y

if __name__ == '__main__':
	connection = psycopg2.connect(user=user, password=password, host=host, port=port, database=database)
	cursor = connection.cursor()
	cursor.execute('''
		select istation, idate, ilongitude, ilatitude, 
		       fstation, fdate, flongitude, flatitude 
		from public.a_carris_whole_trip_val v
		inner join (
				select cod_paragem as icod_paragem,
				latitude as ilatitude,
				longitude as ilongitude
				from public.carris_paragens
			) i
			on v.istation = i.icod_paragem
		inner join (
				select cod_paragem as fcod_paragem,
				latitude as flatitude,
				longitude as flongitude
				from public.carris_paragens
			) f
			on v.fstation = f.fcod_paragem
		where fstation is not null and fdate is not null;
	''')
	result = cursor.fetchall()
	print(f'Number of validations in DB: {len(result)}')

	net = utils.json_utils.read_json_object(configs.NETWORK)

	max_lat = -np.inf
	min_lat = np.inf
	max_lon = -np.inf
	min_lon = np.inf
	_DIVISIONS = 30
	
	MORNING_RUSH_HOUR_START = datetime.datetime.strptime(
		configs.MORNING_RUSH_HOUR_START, '%H:%M'
	).time()

	MORNING_RUSH_HOUR_END = datetime.datetime.strptime(
		configs.MORNING_RUSH_HOUR_END, '%H:%M'
	).time()

	AFTERNOON_RUSH_HOUR_START = datetime.datetime.strptime(
		configs.AFTERNOON_RUSH_HOUR_START, '%H:%M'
	).time()

	AFTERNOON_RUSH_HOUR_END = datetime.datetime.strptime(
		configs.AFTERNOON_RUSH_HOUR_END, '%H:%M'
	).time()

	HAPPY_HOUR_END = datetime.datetime.strptime(
		configs.HAPPY_HOUR_END, '%H:%M'
	).time()

	# Just assessing the limits of the grid
	for node in net['nodes']:
		lon = node['x']
		lat = node['y']

		max_lat = lat if lat > max_lat else max_lat
		min_lat = lat if lat < min_lat else min_lat
		max_lon = lon if lon > max_lon else max_lon
		min_lon = lon if lon < min_lon else min_lon

	lat_step = (max_lat-min_lat)/_DIVISIONS
	lon_step = (max_lon-min_lon)/_DIVISIONS

	odx_matrix_pairs = [
		(i, j) 
		for i, j in itertools.product(range(_DIVISIONS), range(_DIVISIONS))
	]

	odx_matrix = {(*origin, *destin):{
		'origin': [*origin],
		'destin': [*destin],
		'morning_rush_hour':  0,
		'midday': 0,
		'afternoon_rush_hour': 0,
		'happy_hour': 0,
		'night_time': 0,
		'total': 0,
	} for origin, destin in itertools.product(odx_matrix_pairs, odx_matrix_pairs)}

	istation   = 0 
	idate      = 1 
	ilongitude = 2 
	ilatitude  = 3 
	fstation   = 4 
	fdate      = 5  
	flongitude = 6 
	flatitude  = 7 

	for res in result:
		time_i = res[idate].time()
		i_i, j_i = coord_to_square(res[ilongitude], res[ilatitude])
		time_f = res[fdate].time()
		i_f, j_f = coord_to_square(res[flongitude], res[flatitude])
		
		odx_matrix[(i_i, j_i, i_f, j_f)]
		
		if time_i > MORNING_RUSH_HOUR_START and time_i < MORNING_RUSH_HOUR_END:
			odx_matrix[(i_i, j_i, i_f, j_f)]['morning_rush_hour'] += 1
		elif time_i > MORNING_RUSH_HOUR_END and time_i < AFTERNOON_RUSH_HOUR_START:
			odx_matrix[(i_i, j_i, i_f, j_f)]['midday'] += 1
		elif time_i > AFTERNOON_RUSH_HOUR_START and time_i < AFTERNOON_RUSH_HOUR_END:
			odx_matrix[(i_i, j_i, i_f, j_f)]['afternoon_rush_hour'] += 1
		elif time_i > AFTERNOON_RUSH_HOUR_END and time_i < HAPPY_HOUR_END:
			odx_matrix[(i_i, j_i, i_f, j_f)]['happy_hour'] += 1
		else:
			odx_matrix[(i_i, j_i, i_f, j_f)]['night_time'] += 1

		odx_matrix[(i_i, j_i, i_f, j_f)]['total'] += 1

	odx_matrix = [{
		**item
	} for key, item in odx_matrix.items() if item['total']>0]
	
	utils.json_utils.write_json_object(configs.ODX_MATRIX, odx_matrix)

