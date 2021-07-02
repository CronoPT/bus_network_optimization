'''
| Information regarding each existing metro station.
'''
METRO_STATIONS = '../data/json/metro_stations.json'

'''
| A geojson file so we can visualize the metro network.
| This one holds the shape of the lines.
'''
METRO_SHAPE = '../data/geojson/metro_shapes.geojson'

'''
| A geojson file so we can visualize the metro network.
| This one holds the points corresponding to station
| locations.
'''
METRO_POINTS = '../data/geojson/metro_stations.geojson'

'''
| The station sequence in the blue metro line.
'''
BLUE_LINE = [
	'M50', 'M44', 'M43', 'M01', 
	'M02', 'M03', 'M04', 'M05',
	'M06', 'M07', 'M08', 'M09',
	'M10', 'M11', 'M12', 'M29',
	'M45', 'M46'
]

'''
| The station sequence in the yellow metro line.
'''
YELLOW_LINE = [
	'M42', 'M41', 'M40', 'M39',
	'M38', 'M13', 'M14', 'M15',
	'M16', 'M17', 'M18', 'M10',
	'M19'
] 

'''
| The station sequence in the green metro line.
'''
GREEN_LINE = [
	'M37', 'M13', 'M20', 'M21',
	'M22', 'M23', 'M25', 'M26',
	'M27', 'M28', 'M29', 'M30'
]

'''
| The station sequence in the red metro line.
'''
RED_LINE = [
	'M49', 'M48', 'M47', 'M36',
	'M35', 'M34', 'M33', 'M32',
	'M31', 'M23', 'M17', 'M08'
]

'''
| The road network with all the bus stops already 
| included and clustered. Possible to load this one
| into networkX.
'''
NETWORK = '../data/json/final_network.json'

'''
| The location of each bus stop in the road network.
'''
BUS_STOPS = '../data/json/clustered_stop_locations.json'

'''
| A sequence of stops for each existing route in the current version
| of Lisbon's bus network
'''
BUS_ROUTES = '../data/json/clustered_routes_stop_sequence.json'

'''
| Maximum walkeable ditance between bus stops and metro stations
| or between two bus stops. We do not have information regarding 
| walkeable paths, so this is the haversine distance between the
| target points.
'''
MAX_DIST_METRO_BUS = 300

'''
| A loadeable version of the raod network that includes metro
| and the walking interfaces between every mode of transportation
| 
| NOTE: currently not being used.
'''
NETWORK_WITH_METRO = '../data/json/network_with_metro.json'

'''
| A geojson file to allow the visualization of the road network
| with the metro included.
| This one allows for the visualization of the links.
'''
NETWORK_LINKS = '../data/geojson/network_with_metro_links.geojson'

'''
| A geojson file to allow the visualization of the road network
| with the metro included.
| This one allows for the visualization of the nodes.
'''
NETWORK_NODES = '../data/geojson/network_with_metro_nodes.geojson'

'''
| Hold information about which bus stops are reacheable from each
| metro station.
'''
INTERFACED_METRO = '../data/json/metro_stations_interfaced.json'

'''
| Hold information about which metro stations and
| bus stops are reacheable from each bus stop.
'''
INTERFACED_BUS = '../data/json/clustered_stop_locations_interfaced.json'

'''
| For each metro line, hold the sequence of stations
| tha make up the line.
'''
METRO_NETWORK = '../data/json/metro_network.json'

'''
| The speed at which metro works (Km/h)
'''
METRO_SPEED = 60

'''
| The speed at which people walk (Km/h)
'''
WALKING_SPEED = 5

'''
| A small NetworkX network holding the connectivity in 
| Lisbon's metro network. 
'''
METRO_NETWORKNX = '../data/json/metro_networknx.json'

'''
| When sorting stations in grids, adjust the size of
| the grid.
'''
DIVISIONS = 50

'''
| For each metro-station/bus-stop, which other
| stations/stops are withing walkeable distance.
'''
WALKS = '../data/json/walks.json'

'''
| The result of the bus stop clustering process. 
| In case it is needed, this file holds, for each
| removed stop, the cluster which it joined (an
| existing bus stop)
'''
STOP_REPLACEMENTS = '../data/json/stop_replacements.json'

'''
| A list of route sequences generated to serve as a base to
| the genetic algorithms that will be run in this work.
'''
ROUTE_POOL = '../data/json/route_pool.json' 


'''
| All the time stamps bellow work as landmarks to devide
| the day into stages. I'm naming them as follows
| 
| 07:00 (Morning Rush Hour Start)
|   |
|   | Morning Rush Hour
|   |
| 10:00 (Morning Rush Hour End)
|   |
|   | Midday
|   |
| 16:00 (Afternoon Rush Hour Start)
|   |
|   | Afternoon Rush Hour
|   |
| 19:00 (Afternoon Rush Hour End)
|   |
|   | Happy Hour
|   |
| 23:00 (Happy Hour End)
|   |
|   | Night Time
|   |
| 07:00 (Morning Rush Hour Start)
|
'''
MORNING_RUSH_HOUR_START = '07:00'

MORNING_RUSH_HOUR_END = '10:00'

AFTERNOON_RUSH_HOUR_START = '16:00'

AFTERNOON_RUSH_HOUR_END = '19:00'

HAPPY_HOUR_END = '23:00'

ODX_MATRIX = '../data/json/odx_matrix.json'

'''
| These are routes served by trams whihc is a mode of transportation
| we are ignoring in this work.
'''
TRAM_ROUTES = [
	# '15E',
	# '12E',
	# '18E',
	# '24E',
	# '25E',
	# '28E'
]

WALKS_LINES  = '../data/geojson/walk_lines.geojson'
WALKS_POINTS = '../data/geojson/walk_points.geojson'