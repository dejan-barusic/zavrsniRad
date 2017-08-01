#include "stdafx.h"


void loadCSV(const std::string &path, std::vector<std::string> &keys, std::vector<vsite::Country> &countries)
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
		auto c = vsite::Country(name, keys, values);
		countries.push_back(c);
	}
}

char inputCommand() {
	std::cout << "Please select command > ";
	std::string input;
	std::getline(std::cin, input);
	char action;
	if (input.length() == 0)
		action = 'x';
	else
		action = input.at(0);
	return action;
}

std::string inputString() {
	std::string input;
	std::getline(std::cin, input);
	while (input.empty()) {
		std::cout << "Please enter text > ";
		std::getline(std::cin, input);
	}
	return input;
}

std::vector<std::string> inputStrings() {
	using namespace std;
	string line;
	getline(cin, line);
	istringstream iss(line);
	locale L(locale::classic(), new csvCtype);
	iss.imbue(L);
	vector<string> strings;
	copy(istream_iterator<string>(iss), istream_iterator<string>(), back_inserter(strings));
	return strings;
}

double inputDouble() {
	double value;
	std::cin >> value;
	while (std::cin.fail()) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Unable to interpret input. Please enter a number > ";
		std::cin >> value;
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return value;
}

void displayMenu() {
	using namespace std;
	cout << endl;
	cout << "\t[1] Select countries by name" << endl;
	cout << "\t[2] Select countries by column values" << endl;
	cout << "\t[3] Sort selection alphabetically" << endl;
	cout << "\t[4] Sort selection by column name" << endl;
	cout << "\t[5] Display selection" << endl;
	cout << "\t[r] Clear selection" << endl;
	cout << "\t[c] Clear screen" << endl;
	cout << "\t[e] Exit" << endl;
	cout << endl << endl;
}

int main()
{
	using namespace std;

	vector<string> keys;
	vector<vsite::Country> countries;

	loadCSV("zavrsnidb.csv", keys, countries);
	vsite::DataProcessor dp(keys, countries);
	displayMenu();

	while (true) {
		switch (inputCommand())
		{
			case '1':  // Select countries by name
			{
				cout << "Enter country name > ";
				string name = inputString();
				dp.selectByName(name);
				dp.outSelection();
				break;
			}
			case '2':  // Select countries by column values
			{
				cout << "Enter column name > ";
				string column = inputString();
				cout << "Enter min value > ";
				double min = inputDouble();
				cout << "Enter max value > ";
				double max = inputDouble();
				dp.selectByColumnValue(column, min, max);
				dp.outSelection(column);
				break;
			}
			case '3':  // Sort selection alphabetically
				dp.sortAlphabetically();
				break;
			case '4':  // Sort selection by column name
			{
				cout << "Enter column name > ";
				string column = inputString();
				dp.sortByColumnName(column);
				break;
			}
			case '5':  // Display selection
			{
				cout << "Enter column names separated by comma or empty for all columns > ";
				vector<string> columns = inputStrings();
				dp.outSelection(columns);
				break;
			}
			case 'r':  // Clear selection
				dp.clear();
				break;
			case 'c':  // Clear screen
				system("cls");
				displayMenu();
				break;
			case 'e':  // Exit
				return 0;
			default:
				cout << "Unknown command. Please try again." << endl;
				break;
			}
	};

	return 0;
}




