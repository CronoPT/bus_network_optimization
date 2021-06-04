import psycopg2
import configs
import utils.json_utils
import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.colors as colors
import datetime
import pandas as pd
# from sqlalchemy import create_engine
	
user = 'postgres'
password = 'rootroot'
host = '127.0.0.1'
port = '5432'
database = 'iludb'

def get_hours_between(time1, time2):
	date_time_1 = datetime.datetime.combine(datetime.date.today(), time1)
	date_time_2 = datetime.datetime.combine(datetime.date.today(), time2)
	difference = date_time_1 - date_time_2
	difference_hours = difference.total_seconds() / 3600
	return abs(difference_hours)

if __name__ == '__main__':
	connection = psycopg2.connect(user=user, password=password, host=host, port=port, database=database)
	cursor = connection.cursor()
	cursor.execute('''
		select r.cod_rota, p.tempo, p.id_viagem
		from public.carris_percursos p
		inner join public.carris_viagens v 
			on p.id_viagem = v.id_viagem
		inner join public.carris_rotas r
			on v.id_rota = r.id_rota
	''')
	result = cursor.fetchall()

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
	
	cod_rota = 0
	time = 1
	id_trip = 2

	starts = {}
	for r in result:
		starts[r[cod_rota]] = {}
	for r in result:
		starts[r[cod_rota]][r[id_trip]] = []

	for r in result:
		starts[r[cod_rota]][r[id_trip]].append(r[time])

	for cod, trips in starts.items():
		for trip, times in trips.items():
			times.sort()
			starts[cod][trip] = times[0]

	for cod, trips in starts.items():
		for trip, times in trips.items():
			print(times.strftime("%H:%M:%S"))

	routes = {}
	for cod, trips in starts.items():
		routes[cod] = {
			'morning_rush_hour': 0,
			'midday': 0,
			'afternoon_rush_hour': 0,
			'happy_hour': 0,
			'night_time': 0,
			'total': 0
		}
		for trip, time in trips.items():
			if time > MORNING_RUSH_HOUR_START and time < MORNING_RUSH_HOUR_END:
				routes[cod]['morning_rush_hour'] += 1
			elif time > MORNING_RUSH_HOUR_END and time < AFTERNOON_RUSH_HOUR_START:
				routes[cod]['midday'] += 1
			elif time > AFTERNOON_RUSH_HOUR_START and time < AFTERNOON_RUSH_HOUR_END:
				routes[cod]['afternoon_rush_hour'] += 1
			elif time > AFTERNOON_RUSH_HOUR_END and time < HAPPY_HOUR_END:
				routes[cod]['happy_hour'] += 1
			else:
				routes[cod]['night_time'] += 1
			routes[cod]['total'] += 1

			print(times.strftime("%H:%M:%S"))

	MORNING_RUSH_HOUR_DURATION = get_hours_between(MORNING_RUSH_HOUR_START, MORNING_RUSH_HOUR_END)
	MIDDAY_DURATION = get_hours_between(MORNING_RUSH_HOUR_END, AFTERNOON_RUSH_HOUR_START)
	AFTERNOON_RUSH_HOUR_DURATION = get_hours_between(AFTERNOON_RUSH_HOUR_START, AFTERNOON_RUSH_HOUR_END)
	HAPPY_HOUR_DURATION = get_hours_between(AFTERNOON_RUSH_HOUR_END, HAPPY_HOUR_END)
	NIGHT_TIME_DURATION = 24 - MORNING_RUSH_HOUR_DURATION \
	                         - MIDDAY_DURATION \
							 - AFTERNOON_RUSH_HOUR_DURATION \
							 - HAPPY_HOUR_DURATION

	print(f'Hours in the morning rush hour: {MORNING_RUSH_HOUR_DURATION}')
	print(f'Hours in the midday portion: {MIDDAY_DURATION}')
	print(f'Hours in the afternoon rush hour: {AFTERNOON_RUSH_HOUR_DURATION}')
	print(f'Hours in the happy hour: {HAPPY_HOUR_DURATION}')
	print(f'Hours in the night time: {NIGHT_TIME_DURATION}')

	for cod, item in routes.items():
		item['morning_rush_hour'] /= MORNING_RUSH_HOUR_DURATION
		item['midday'] /= MIDDAY_DURATION
		item['afternoon_rush_hour'] /= AFTERNOON_RUSH_HOUR_DURATION
		item['happy_hour'] /= HAPPY_HOUR_DURATION
		item['night_time'] /= NIGHT_TIME_DURATION
		item['total'] /= 24

	utils.json_utils.write_json_object('../data/json/frequencies.json', routes)

	json_routes = utils.json_utils.read_json_object('../data/json/clustered_routes_stop_sequence.json')

	for index, cod in enumerate(routes):
		for route in json_routes:
			if cod in route['route_id']:
				for key, val in routes[cod].items():
					route[key] = val

	for route in json_routes:
		if 'total' not in route:
			route['morning_rush_hour'] = 2
			route['midday'] = 2
			route['afternoon_rush_hour'] = 2
			route['happy_hour'] = 2
			route['night_time'] = 2
			route['total'] = 2

	utils.json_utils.write_json_object('../data/json/frequencies_complete.json', json_routes)

	incomplete = 0
	for i in json_routes:
		if 'total' not in i:
			incomplete += 1

	print(f'Imcompletes: {incomplete}')