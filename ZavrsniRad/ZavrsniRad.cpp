#include "stdafx.h"


void loadCSV(const std::string &path, std::vector<std::string> &keys, std::vector<vsite::Country2> &countries)
{
	std::ifstream file(path);
	std::locale L(std::locale::classic(), new csvCtype);
	file.imbue(L);

	if (file.good()) {
		std::string line;
		std::getline(file, line);
		std::istringstream iss(line);
		iss.imbue(L);
		std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(keys));
	}
	while (file.good()) {
		std::string line, name;
		std::vector<vsite::Field> values;
		std::getline(file, line);
		std::istringstream iss(line);
		iss.imbue(L);
		iss >> name;
		std::copy(std::istream_iterator<vsite::Field>(iss), std::istream_iterator<vsite::Field>(), std::back_inserter(values));
		auto c = vsite::Country2(name, keys, values);
		countries.push_back(c);
	}
}

int main()
{
	using namespace std;

	vector<string> keys;
	vector<vsite::Country2> countries;

	loadCSV("zavrsnidb.csv", keys, countries);

	vsite::DataProcessor dp(keys, countries);
	vector<string> sk{ "life", "", "grah", "area"};
	dp.out("br", sk);

	return 0;











	list<vsite::Country> allCountries, selectedCountries;

	ifstream file("zavrsnidb.csv");
	string rowKeys;

	if (file.good()) {
		getline(file, rowKeys);
		keys = vsite::Tokenizer::tokenize(rowKeys, ';');
		keys.at(0) = "";
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


	displayMenu();

	while (true) {
		cout << "Please select command: ";
		string input;
		getline(cin, input);
		char action;
		if (input.length() == 0)
			action = 'x';
		else
			action = input.at(0);
		if (action == 'e')
			break;

		switch (action)
		{
		// Select countries by name
		case '1':
		{
			cout << "Enter part of country's name: ";
			string input;
			getline(cin, input);
			selectedCountries = (selectCountriesByPartialName(allCountries, input));
			displayCountriesNames(selectedCountries);
			break;
		}
		// Select countries by data column
		case '2':
		{
			cout << "Enter column name: ";
			string validKey = readColumnName(keys);
			cout << "Enter minimum value: ";
			double min = readDouble();
			cout << "Enter maximum value: ";
			double max = readDouble();
			selectedCountries = selectCountriesByData(allCountries, validKey, min, max);
			selectedCountries = sortCountriesByData(selectedCountries, validKey);
			displayCountriesData(selectedCountries, validKey);
			break;
		}
		// Sort selected countries by data column
		case '3':
		{
			cout << "Enter column name: ";
			string validKey = readColumnName(keys);
			selectedCountries = sortCountriesByData(selectedCountries, validKey);
			displayCountriesData(selectedCountries, validKey);
			break;
		}
		// Calculate avarage value of data column for selected countries
		case '4':
		{
			cout << "Enter column name: ";
			string validKey = readColumnName(keys);
			double avarage = calculateAvarageByKey(selectedCountries, validKey);
			displayCountriesData(selectedCountries, validKey);
			cout << "Avarage value for column \"" << validKey << "\" is " << avarage << endl << endl;
			break;
		}
		// Find selected country with minimal column value
		case '5':
		{
			cout << "Enter column name: ";
			string validKey = readColumnName(keys);
			list<vsite::Country> country; 
			country.push_back(findSingleCountryWithMinValue(allCountries, validKey));
			displayCountriesData(country, validKey);
			break;
		}
		// Show selected countries
		case '6':
			displayCountriesNames(selectedCountries);
			break;
		// Show selected countries with data columns
		case '7':
		{
			cout << "Enter column names separated by space: ";
			string input, userKey;
			vector<string> userKeys;
			getline(cin, input);
			istringstream iss(input);
			while (iss >> userKey) {
				string validKey = getValidKey(keys, userKey);
				if (validKey.length() > 0)
					userKeys.push_back(validKey);
				else
					cout << "Column \"" << userKey << "\" doesn't exist." << endl;
			}
			displayCountriesData(selectedCountries, userKeys);
			break;
		}
		// Reverse sorting
		case '8':
			selectedCountries = reverseOrder(selectedCountries);
			displayCountriesNames(selectedCountries);
			break;
		// Select all countries
		case 'a':
			selectedCountries.erase(selectedCountries.begin(), selectedCountries.end());
			copy(allCountries.begin(), allCountries.end(), std::back_inserter(selectedCountries));
			break;
		// Clear screen
		case 'c':
			system("cls");
			displayMenu();
			break;
		default:
			cout << "Unknown command. Please try again." << endl;
			break;
		}
	}

    return 0;
}


double readDouble() {
	using namespace std;
	double value;
	cin >> value;
	while (cin.fail()) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Unable to interpret input. Please enter a number: ";
		cin >> value;
	}
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return value;
}

std::string readColumnName(std::vector<std::string> keys) {
	using namespace std;
	string userKey;
	cin >> userKey;
	string validKey = getValidKey(keys, userKey);
	while (validKey.empty()) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Unable to find column. Please enter column name: ";
		cin >> userKey;
		validKey = getValidKey(keys, userKey);
	}
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return validKey;
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
				if (country.name.empty() || i->first == validKey && i->second >= minValue && i->second <= maxValue) {
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
	return result != validKeys.end() ? *result : "";
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

void displayCountriesNames(std::list<vsite::Country> countries) {
	using namespace std;
	if (countries.empty()) {
		cout << endl << "No countries to display." << endl << endl;
		return;
	}
	cout << endl;
	for_each(countries.cbegin(), countries.cend(), 
		[](vsite::Country country) {
			cout << "[" << country.name << "] ";
		});
	cout << endl << endl;
}

void displayCountriesData(std::list<vsite::Country> countries, std::string validKey) {
	std::vector<std::string> validKeys;
	validKeys.push_back(validKey);
	displayCountriesData(countries, validKeys);
}

void displayCountriesData(std::list<vsite::Country> countries, std::vector<std::string> validKeys) {
	using namespace std;
	if (validKeys.empty()) {
		cout << endl << "No columns to display." << endl << endl;
		return;
	}
	const int columnWidthCharsData = 20;
	const int columnWidthCharsName = 25;

	string namePlaceholder(columnWidthCharsName + 1, ' ');

	cout << endl << namePlaceholder;

	for (auto i = validKeys.cbegin(); i != validKeys.cend(); ++i) {
		string resized(*i);
		resized.resize(columnWidthCharsData, ' ');
		cout << resized << ' ';
	}
	cout << endl;

	for_each(countries.cbegin(), countries.cend(),
		[&validKeys, &columnWidthCharsData, &columnWidthCharsName](vsite::Country country) {
			string name(country.name);
			name.resize(columnWidthCharsName, ' ');
			cout << name << ' ';

			for (auto i = validKeys.cbegin(); i != validKeys.cend(); ++i) {
				for (auto j = country.data.cbegin(); j != country.data.cend(); ++j) {
					if (*i == j->first) {
						std::cout << std::resetiosflags(std::ios::adjustfield);
						std::cout << std::setiosflags(std::ios::left);
						std::cout.width(columnWidthCharsData);
						cout << j->second << ' ';
						break;
					}
				}
			}
			cout << endl;
	});
	cout << endl;
}

void displayMenu() {
	using namespace std;
	cout << endl;
	cout << "\t[1] Select countries by name" << endl;
	cout << "\t[2] Select countries by data column" << endl;
	cout << "\t[3] Sort selected countries by data column" << endl;
	cout << "\t[4] Calculate avarage value of data column for selected countries" << endl;
	cout << "\t[5] Find selected country with minimal column value" << endl;
	cout << "\t[6] Show selected countries" << endl;
	cout << "\t[7] Show selected countries with data columns" << endl;
	cout << "\t[8] Reverse sorting" << endl;
	cout << "\t[a] Select all countries" << endl;
	cout << "\t[c] Clear screen" << endl;
	cout << "\t[e] Exit" << endl;
	cout << endl << endl;
}
