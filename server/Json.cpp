#include "Json.hpp"

std::optional<std::string> getString(const json_t* const json, const char* key) {
	json_t* obj = json_object_get(json, key);
	if (!obj) return std::nullopt;
	auto charp = json_string_value(obj);
	if (!charp) return std::nullopt;
	auto str = std::string{charp};
	return str;
}

std::optional<int> getInt(const json_t* const json, const char* key) {
	json_t* obj = json_object_get(json, key);
	if (obj == nullptr) return std::nullopt;
	if (!json_is_integer(obj)) return std::nullopt;
	return (int) json_integer_value(obj);
}

