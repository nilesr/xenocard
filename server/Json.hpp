#pragma once
#include <map>
#include <jansson.h>
#include <string>

template<typename K, typename V>
json_t* map_to_obj(std::map<K, V> map, std::string (*transform_key)(K), json_t* (*transform_val)(V)) {
	json_t* obj = json_object();
	for (const auto [k, v] : map) {
		json_object_set(obj, (*transform_key)(k).c_str(), (*transform_val)(v));
	}
	return obj;
}


