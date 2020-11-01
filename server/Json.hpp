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


std::optional<std::string> getString(const json_t* const json, const char* key);

std::optional<int> getInt(const json_t* const json, const char* key);

class defer_decref {
	json_t* json;
public:
	defer_decref(json_t* json): json(json) {};
	~defer_decref() {
		json_decref(json);
	}
};


