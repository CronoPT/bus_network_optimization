import utils.json_utils

if __name__ == '__main__':
	org_matrix = utils.json_utils.read_json_object('../data/json/odx_matrix_.json')
	new_matrix = utils.json_utils.read_json_object('../data/json/odx_matrix.json')

	org_nets = utils.json_utils.read_json_object('../data/json/run_nets_.json')
	new_nets = utils.json_utils.read_json_object('../data/json/run_nets.json')

	print(f'original -> {len(org_matrix)}')
	print(f'new -> {len(new_matrix)}')

	print(f'original -> {len(org_nets)}')
	print(f'new -> {len(new_nets)}')