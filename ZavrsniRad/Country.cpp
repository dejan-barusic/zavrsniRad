#include "Country.h"


namespace vsite {

	Country::Country(vector<string> keys, vector<string> values)
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

	Country::~Country()
	{
	}

	void Country::operator=(Country other)
	{
		name = other.name;
		for (auto i = other.data.cbegin(); i != other.data.cend(); ++i) {
			data[i->first] = i->second;
		}
	}
}