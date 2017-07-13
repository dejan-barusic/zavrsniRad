#include "stdafx.h"

std::list<vsite::Country> selectCountriesByPartialName(std::list<vsite::Country>, std::string);
std::list<vsite::Country> selectCountriesByData(std::list<vsite::Country>, std::string, double);
std::list<vsite::Country> selectCountriesByData(std::list<vsite::Country>, std::string, double, double);
vsite::Country findSingleCountryWithMinValue(std::list<vsite::Country>, std::string);
std::string getValidKey(std::vector<std::string>, std::string);
std::list<vsite::Country> sortCountriesByData(std::list<vsite::Country>, std::string);
bool checkSorting(std::list<vsite::Country>, std::string);
std::list<vsite::Country> reverseOrder(std::list<vsite::Country>);
double calculateAvarageByKey(std::list<vsite::Country>, std::string);



int main()
{
	using namespace std;

	list<vsite::Country> allCountries, selectedCountries;
	ifstream file("zavrsnidb.csv");
	string rowKeys;
	vector<string> keys;

	if (file.good()) {
		getline(file, rowKeys);
		keys = vsite::Tokenizer::tokenize(rowKeys, ';');
	}

	while (file.good())	{
		vector<string> values;
		string rowValues;
		std::getline(file, rowValues);
		values = vsite::Tokenizer::tokenize(rowValues, ';');
		try {
			if(keys.size() == values.size())
				allCountries.push_back(vsite::Country(keys, values));
			else
				throw invalid_argument("wrong number of data fields for country " + values[0]);
		}
		catch (const invalid_argument& ia) {
			cout << "Error while creating country data: " << ia.what() << endl;
		}
	}

	selectedCountries = selectCountriesByData(allCountries, getValidKey(keys, "life"), 50, 55);
	bool isSortedBefore = checkSorting(selectedCountries, getValidKey(keys, "life"));
	selectedCountries = sortCountriesByData(selectedCountries, getValidKey(keys, "life"));
	bool isSortedAfter = checkSorting(selectedCountries, getValidKey(keys, "life"));
	list<vsite::Country> reversedCountries = reverseOrder(selectedCountries);
	double avg = calculateAvarageByKey(selectedCountries, getValidKey(keys, "life"));
	vsite::Country c = findSingleCountryWithMinValue(allCountries, getValidKey(keys, "life"));
	string s = getValidKey(keys, "elect");

    return 0;
}




std::list<vsite::Country> selectCountriesByPartialName(std::list<vsite::Country> countries, std::string name) {
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	auto separator = remove_if(countries.begin(), countries.end(), 
						[&name](vsite::Country country) {
							std::string iname = country.name;
							std::transform(iname.begin(), iname.end(), iname.begin(), ::tolower);
							return iname.find(name) == std::string::npos;
						});
	countries.erase(separator, countries.end());
	return countries;
}

std::list<vsite::Country> selectCountriesByData(std::list<vsite::Country> countries, std::string validKey, double exactValue) {
	return selectCountriesByData(countries, validKey, exactValue, exactValue);
}

std::list<vsite::Country> selectCountriesByData(std::list<vsite::Country> countries, std::string validKey, double minValue, double maxValue) {
	auto separator = remove_if(countries.begin(), countries.end(),
		[&validKey, minValue, maxValue](vsite::Country country) {
			bool flag = true;		
			for (auto i = country.data.begin(); i != country.data.end(); ++i) {
				if (i->first == validKey && i->second >= minValue && i->second <= maxValue) {
					flag = false;
					break;
				}
			}
			return flag;
	});
	countries.erase(separator, countries.end());
	return countries;
}

vsite::Country findSingleCountryWithMinValue(std::list<vsite::Country> countries, std::string validKey) {
	auto result = std::min_element(countries.cbegin(), countries.cend(),
		[&validKey](vsite::Country left, vsite::Country right) {
			return left.data[validKey] < right.data[validKey];
	});
	return *result;
}

std::string getValidKey(std::vector<std::string> validKeys, std::string partialKey) {
	std::transform(partialKey.begin(), partialKey.end(), partialKey.begin(), ::tolower);
	auto result = std::find_if(validKeys.cbegin(), validKeys.cend(),
		[&partialKey](std::string validKey) {
			std::transform(validKey.begin(), validKey.end(), validKey.begin(), ::tolower);
			return validKey.find(partialKey) != std::string::npos;
		});
	return *result;
}

/*
Pošto std::sort koristi random access iteratore koje list ne podržava napunio sam vector sa pointerima na objekte,
sortirao vector i napunio novi list preko sortiranih pointera. 
Razumnije je koristiti member sort od liste ali ovo za je primjer STL sortiranja.
*/
std::list<vsite::Country> sortCountriesByData(std::list<vsite::Country> countries, std::string validKey) {
	std::vector<vsite::Country*> pointersToCountries{};
	for (auto i = countries.begin(); i != countries.end(); ++i) {
		pointersToCountries.push_back(&(*i));
	}
	std::sort(pointersToCountries.begin(), pointersToCountries.end(),
		[&validKey](vsite::Country* left, vsite::Country* right) {
			return left->data[validKey] < right->data[validKey];
	});
	std::list<vsite::Country> sortedCountries{};
	for (auto i : pointersToCountries) {
		sortedCountries.push_back(*i);
	}
	return sortedCountries;
}

bool checkSorting(std::list<vsite::Country> countries, std::string validKey) {
	return std::is_sorted(countries.cbegin(), countries.cend(), 
		[&validKey](vsite::Country left, vsite::Country right) {
			return left.data[validKey] < right.data[validKey];
	});
}

std::list<vsite::Country> reverseOrder(std::list<vsite::Country> countries) {
	std::reverse(countries.begin(), countries.end());
	return countries;
}

double calculateAvarageByKey(std::list<vsite::Country> countries, std::string validKey) {
	return std::accumulate(countries.cbegin(), countries.cend(), 0.0, 
		[&validKey](double left, vsite::Country right) {
			return left + right.data[validKey];
		}) / countries.size();
}