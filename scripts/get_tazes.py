from typing import final
import geopandas
import utils.json_utils


if __name__ == '__main__':
	shp_file = geopandas.read_file('../data/Zonamento_good/Zonamento_zone_Project.shp')
	shp_file.to_file('../data/geojson/tazes.geojson', driver='GeoJSON')

	json = utils.json_utils.read_json_object('../data/geojson/tazes.geojson')

	final_features = []
	for feature in json['features']:
		if feature['properties']['TYPENO'] == '1':
			final_features.append(feature)

	json['features'] = final_features

	utils.json_utils.write_json_object('../data/geojson/tazes.geojson', json)