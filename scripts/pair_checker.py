import sys
import utils.json_utils

if __name__ == '__main__':
	INDEX = int(sys.argv[1])
	SHOW  = False 

	json = utils.json_utils.read_json_object(f'../data/json/comparisons/comparison-{INDEX}.json')
	odx_json = utils.json_utils.read_json_object(f'../data/json/odx_matrix.json')

	dict = {}
	odx_dict = {}

	for obj in json:
		origin = obj['origin']
		destin = obj['destin']

		if (*destin, *origin) in dict:
			dict[(*destin, *origin)] = True
			dict[(*origin, *destin)] = True
		else:
			dict[(*origin, *destin)] = False

	mirrors = 0
	for pair, check in dict.items():
		if check:
			mirrors += 1

	for obj in odx_json:
		origin = obj['origin']
		destin = obj['destin']

		if (*destin, *origin) in odx_dict:
			odx_dict[(*destin, *origin)] = True
			odx_dict[(*origin, *destin)] = True
		else:
			odx_dict[(*origin, *destin)] = False

	odx_mirrors = 0
	for pair, check in odx_dict.items():
		if check:
			odx_mirrors += 1

	print(f'Total: {len(dict)} | Mirrors: {mirrors}')
	print(f'Total: {len(odx_dict)} | Mirrors: {odx_mirrors}')