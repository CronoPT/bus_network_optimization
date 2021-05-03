import psycopg2
import configs
import utils.json_utils
import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import datetime

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

	heat_map_i = [[0 for _ in range(_DIVISIONS)] for _ in range(_DIVISIONS)]
	heat_map_f = [[0 for _ in range(_DIVISIONS)] for _ in range(_DIVISIONS)]
	heat_stop  = {}

	istation   = 0 
	idate      = 1 
	ilongitude = 2 
	ilatitude  = 3 
	fstation   = 4 
	fdate      = 5  
	flongitude = 6 
	flatitude  = 7 

	_lower_time = datetime.time(19, 0)
	_upper_time = datetime.time(23, 0)
	for res in result:
		time_i = res[idate].time()
		i_i, j_i = coord_to_square(res[ilongitude], res[ilatitude])
		time_f = res[fdate].time()
		i_f, j_f = coord_to_square(res[flongitude], res[flatitude])

		if res[istation] not in heat_stop:
			heat_stop[res[istation]] = 0
		if res[fstation] not in heat_stop:
			heat_stop[res[fstation]] = 0

		heat_stop[res[istation]] += 1
		heat_stop[res[fstation]] += 1				

		if not (i_i == i_f and j_i == j_f):
			if time_i > _lower_time and time_i < _upper_time:
				heat_map_i[_DIVISIONS-1-j_i][i_i] += 1

			if time_f > _lower_time and time_f < _upper_time:
				heat_map_f[_DIVISIONS-1-j_f][i_f] += 1
	

	top_stops = [(stop, count) for stop, count in heat_stop.items()]
	top_stops.sort(key = lambda x : x[1], reverse=True)

	for info in top_stops[:10]:
		print(f'Stop: {info[0]} | Validations: {info[1]}')

	fig, ax = plt.subplots(1, 2)
	im_i = ax[0].imshow(heat_map_i, cmap='Purples', norm=colors.PowerNorm(0.3))
	ax[0].set_title('Entradas')
	im_f = ax[1].imshow(heat_map_f, cmap='Purples', norm=colors.PowerNorm(0.3))
	ax[1].set_title('Saídas')
	plt.suptitle(f'Entradas e Saídas entre as {_lower_time:%H:%M} e as {_upper_time:%H:%M}')
	# plt.show()