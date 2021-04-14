import psycopg2
import configs
import utils.json_utils

user = 'postgres'
password = 'rootroot'
host = '127.0.0.1'
port = '5432'
database = 'iludb'

if __name__ == '__main__':
	connection = psycopg2.connect(user=user, password=password, host=host, port=port, database=database)
	cursor = connection.cursor()
	cursor.execute('''
		select cod_paragem, nome, latitude, longitude 
		from public.metro_paragens;
	''')
	result = cursor.fetchall()

	cod_paragem = 0
	nome = 1
	latitude  = 2
	longitude = 3

	metro_stations = {}
	points = []
	for item in result:
		point = [
			item[longitude],
			item[latitude]
		]
		metro_stations[item[cod_paragem]] = {
			'nome':  item[nome],
			'point': point
		}
		points.append(point)

	blue   = configs.BLUE_LINE
	yellow = configs.YELLOW_LINE
	green  = configs.GREEN_LINE
	red    = configs.RED_LINE

	mlines = [blue, yellow, green, red]
	lines  = []
	for mline in mlines:
		line = []
		for stop in mline:
			line.append(metro_stations[stop]['point'])
		lines.append(line)

	utils.json_utils.write_geojson_points(configs.METRO_POINTS, points)
	utils.json_utils.write_geojson_lines(configs.METRO_SHAPE, lines)
	utils.json_utils.write_json_object(configs.METRO_STATIONS, metro_stations)