#include "Country.h"

namespace vsite {

	Country::Country(const std::string name, std::vector<std::string> &keys, const std::vector<vsite::Field> values) : name(name), keys(keys), values(values) {
	}

	void Country::operator=(const Country &other) {
		name = other.name;
		keys = other.keys;
		values = other.values;
	}

}