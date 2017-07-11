#include "Country.h"
#include <iostream>
#include <string>

#include <algorithm>
#include <iterator>
#include <vector>

namespace vsite {

	Country::Country(const std::string input)
	{
		using namespace std;
		vector<string> rawData {};
		auto fbegin = input.cbegin();
		auto fend = input.cbegin();

		for (auto i = input.cbegin(); i != input.cend(); ++i) {
			if (*i == ';') {
				rawData.push_back(string(fbegin, fend));
				++fend;
				fbegin = fend;
			}
			else
				++fend;
		}
		rawData.push_back(string(fbegin, fend));

		name = rawData[0];
		try {
			data["Area(sq km)"] = stod(rawData[1]);
			data["Population"] = stod(rawData[2]);
			data["Life expectancy at birth(years)"] = stod(rawData[3]);
			data["Birth rate(births/1000 population)"] = stod(rawData[4]);
			data["Death rate(deaths/1000 population)"] = stod(rawData[5]);
			data["Unemployment rate(%)"] = stod(rawData[6]);
			data["Internet users"] = stod(rawData[7]);
			data["Current account balance"] = stod(rawData[8]);
			data["GDP"] = stod(rawData[9]);
			data["GDP - per capita"] = stod(rawData[10]);
			data["Debt - external"] = stod(rawData[11]);
			data["Public debt(% of GDP)"] = stod(rawData[12]);
			data["Electricity - consumption(kWh)"] = stod(rawData[13]);
			data["Electricity - production(kWh)"] = stod(rawData[14]);
		}
		catch (const std::invalid_argument& ia) {
			std::cerr << "Invalid argument: " << ia.what() << '\n';
		}
	}


	Country::~Country()
	{
	}

}