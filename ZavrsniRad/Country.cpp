#include "Country.h"


namespace vsite {

	Country::Country(vector<string> keys, vector<string> values)
	{
		
		name = values[0];

		for (int i = 1; i < keys.size(); ++i) {
			try {
				data[keys[i]] = stod(values[i]);
			}
			catch (const invalid_argument& ia) {
				throw invalid_argument(name + "'s data contains invalid numbers");
			}
		}
	}


	Country::~Country()
	{
	}

}