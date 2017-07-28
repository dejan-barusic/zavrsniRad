#include "Country.h"


namespace vsite {

	Country::Country(const vector<string> &keys, vector<string> values)
	{
		
		name = values[0];

		for (unsigned i = 1; i < keys.size(); ++i) {
			try {
				data[keys[i]] = stod(values[i]);
			}
			catch (const invalid_argument) {
				throw invalid_argument(name + "'s data contains invalid numbers");
			}
		}
	}


	Country2::Country2(const std::string name, std::vector<std::string> &keys, const std::vector<vsite::Field> values) : name(name), keys(keys), values(values)
	{
	}

	void Country2::operator=(const Country2 &other)
	{
		name = other.name;
		keys = other.keys;
		values = other.values;
	}


}